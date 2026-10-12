#include "Huffman.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <istream>
#include <memory>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <vector>

using namespace std;

namespace {
struct Nodo {
    uint64_t frecuencia;
    int simbolo, minimo;
    shared_ptr<Nodo> izquierdo, derecho;
};
using Ptr = shared_ptr<Nodo>;
using Frecuencias = array<uint64_t, 256>;
struct Comparar {
    bool operator()(const Ptr& a, const Ptr& b) const {
        if (a->frecuencia != b->frecuencia) return a->frecuencia > b->frecuencia;
        return a->minimo > b->minimo;
    }
};
Ptr construir(const Frecuencias& f) {
    priority_queue<Ptr, vector<Ptr>, Comparar> cola;
    for (int i = 0; i < 256; ++i)
        if (f[i]) cola.push(make_shared<Nodo>(Nodo{f[i], i, i, nullptr, nullptr}));
    while (cola.size() > 1) {
        Ptr a = cola.top(); cola.pop();
        Ptr b = cola.top(); cola.pop();
        cola.push(make_shared<Nodo>(Nodo{a->frecuencia + b->frecuencia, -1,
                  min(a->minimo, b->minimo), a, b}));
    }
    return cola.empty() ? nullptr : cola.top();
}
void codigos(const Ptr& n, const string& prefijo, array<string, 256>& c) {
    if (!n) return;
    if (n->simbolo >= 0) { c[n->simbolo] = prefijo.empty() ? "0" : prefijo; return; }
    codigos(n->izquierdo, prefijo + '0', c);
    codigos(n->derecho, prefijo + '1', c);
}
void escribir(ostream& s, uint64_t n, int bytes) {
    for (int i = 0; i < bytes; ++i) s.put(static_cast<char>((n >> (8 * i)) & 255));
}
uint64_t leer(istream& s, int bytes) {
    uint64_t n = 0;
    for (int i = 0; i < bytes; ++i) {
        int c = s.get();
        if (c == char_traits<char>::eof()) throw runtime_error("Archivo Huffman truncado");
        n |= uint64_t(static_cast<unsigned char>(c)) << (8 * i);
    }
    return n;
}
}
void Huffman::comprimir(const string& texto, ostream& salida) {
    Frecuencias f{};
    for (unsigned char c : texto) ++f[c];
    unsigned n = 0;
    for (uint64_t valor : f) if (valor) ++n;
    escribir(salida, n, 2);
    for (int i = 0; i < 256; ++i) if (f[i]) {
        escribir(salida, i, 1); escribir(salida, f[i], 8);
    }
    escribir(salida, texto.size(), 8);
    array<string, 256> c;
    codigos(construir(f), "", c);
    unsigned byte = 0, usados = 0;
    for (unsigned char simbolo : texto) for (char bit : c[simbolo]) {
        byte = (byte << 1) | unsigned(bit == '1');
        if (++usados == 8) { salida.put(static_cast<char>(byte)); usados = byte = 0; }
    }
    if (usados) salida.put(static_cast<char>(byte << (8 - usados)));
    if (!salida) throw runtime_error("No se pudo escribir Huffman");
}
string Huffman::descomprimir(istream& entrada) {
    constexpr uint64_t limite = 4 * 1024 * 1024;
    uint64_t n = leer(entrada, 2), suma = 0;
    if (n > 256) throw runtime_error("Cantidad de simbolos invalida");
    Frecuencias f{}, reales{};
    for (uint64_t i = 0; i < n; ++i) {
        uint64_t c = leer(entrada, 1), valor = leer(entrada, 8);
        if (!valor || valor > limite || f[c] || suma > limite - valor)
            throw runtime_error("Frecuencias invalidas");
        f[c] = valor; suma += valor;
    }
    uint64_t largo = leer(entrada, 8);
    if (largo != suma) throw runtime_error("Longitud invalida");
    Ptr raiz = construir(f);
    string texto;
    texto.reserve(static_cast<size_t>(largo));
    unsigned byte = 0, restantes = 0;
    function<unsigned()> bit = [&]() {
        if (!restantes) { byte = static_cast<unsigned>(leer(entrada, 1)); restantes = 8; }
        return (byte >> --restantes) & 1;
    };
    while (texto.size() < largo) {
        Ptr nodo = raiz;
        if (nodo->simbolo >= 0) {
            if (bit()) throw runtime_error("Codigo unitario invalido");
        } else {
            while (nodo->simbolo < 0) nodo = bit() ? nodo->derecho : nodo->izquierdo;
        }
        texto.push_back(static_cast<char>(nodo->simbolo));
        ++reales[nodo->simbolo];
    }
    if (reales != f || (restantes && (byte & ((1u << restantes) - 1))) ||
        entrada.peek() != char_traits<char>::eof())
        throw runtime_error("Datos Huffman inconsistentes");
    return texto;
}
