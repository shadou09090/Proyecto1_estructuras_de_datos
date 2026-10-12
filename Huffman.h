#ifndef RISK_HUFFMAN_H
#define RISK_HUFFMAN_H
#include <string>
#include <iosfwd>

using namespace std;
namespace Huffman {
void comprimir(const string& texto, ostream& salida);
string descomprimir(istream& entrada);
}
#endif
