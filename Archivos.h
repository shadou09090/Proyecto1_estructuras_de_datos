#ifndef RISK_ARCHIVOS_H
#define RISK_ARCHIVOS_H
#include <cerrno>
#include <cstdio>
#include <string>
#include <system_error>
#include <sys/stat.h>

using namespace std;

namespace Archivos {
inline bool consultar(const string& ruta, struct stat& datos) {
    if (::stat(ruta.c_str(), &datos) == 0) return true;
    const int error = errno;
    if (error == ENOENT || error == ENOTDIR) return false;
    throw system_error(error, generic_category(), "No se pudo consultar " + ruta);
}
inline bool existe(const string& ruta) {
    struct stat datos;
    return consultar(ruta, datos);
}
inline bool esArchivo(const string& ruta) {
    struct stat datos;
    return consultar(ruta, datos) && (datos.st_mode & S_IFMT) == S_IFREG;
}
inline void renombrar(const string& origen, const string& destino) {
    if (rename(origen.c_str(), destino.c_str()) != 0)
        throw system_error(errno, generic_category(), "No se pudo renombrar " + origen);
}
inline bool intentarRenombrar(const string& origen, const string& destino) {
    return rename(origen.c_str(), destino.c_str()) == 0;
}
inline bool intentarEliminar(const string& ruta) {
    return remove(ruta.c_str()) == 0;
}
}
#endif
