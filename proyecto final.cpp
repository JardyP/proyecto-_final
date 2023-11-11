#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

struct Producto {
    string nombre;
    int codigo;
    double precio;
    int existencia;
};

struct Cliente {
    string nombre;
    string direccion;
};

void mostrarInventario(const vector<Producto>& inventario) {
    cout << "Inventario:\n";
    cout << "Codigo\tNombre\tPrecio\tExistencia\n";
    for (const auto& producto : inventario) {
        cout << producto.codigo << "\t" << producto.nombre << "\tQ" << producto.precio << "\t" << producto.existencia << endl;
    }
}

Producto seleccionarProducto(const vector<Producto>& inventario, int codigo) {
    for (const auto& producto : inventario) {
        if (producto.codigo == codigo && producto.existencia > 0) {
            return producto;
        }
    }
    return {"N/A", 0, 0.0, 0};
}

void generarFactura(const Cliente& cliente, const Producto& producto, int cantidad, double precioTotal) {
    const double ivaPorcentaje = 0.12;
    double precioTotalIva = precioTotal * ivaPorcentaje;

    time_t tiempoActual = time(0);
    tm* tiempoLocal = localtime(&tiempoActual);

    cout << "\nFactura:\n";
    cout << "Fecha: " << (tiempoLocal->tm_year + 1900) << '-' << (tiempoLocal->tm_mon + 1) << '-' << tiempoLocal->tm_mday << endl;
    cout << " Hora: " << tiempoLocal->tm_hour << ':' << tiempoLocal->tm_min << ':' << tiempoLocal->tm_sec << endl;
    cout << "Cliente: " << cliente.nombre << "\nDireccion: " << cliente.direccion << endl;
    cout << "\nProducto seleccionado:\n";
    cout << "Codigo: " << producto.codigo << "\tNombre: " << producto.nombre << "\tPrecio: Q" << producto.precio << endl;
    cout << "Cantidad comprada: " << cantidad << "\tPrecio Total: Q" << precioTotal << "\tPrecio Total (con IVA 12%): Q" << precioTotal + precioTotalIva << endl;
}

void guardarDatos(const Cliente& cliente, const Producto& producto, int cantidad, double precioTotal) {
    ofstream archivo("registro_compras.txt", ios::app);
    const double ivaPorcentaje = 0.12;
    double precioTotalIva = precioTotal * ivaPorcentaje;

    archivo << "Cliente: " << cliente.nombre << "\tDireccion: " << cliente.direccion << endl;
    archivo << "Producto: " << producto.nombre << "\tCodigo: " << producto.codigo << "\tPrecio: Q" << producto.precio << endl;
    archivo << "Cantidad comprada: " << cantidad << "\tPrecio Total: Q" << precioTotal << "\tPrecio Total (con IVA 12%): Q" << precioTotal + precioTotalIva << endl;
    archivo << "-------------------------\n";
    archivo.close();
}

void agregarProducto(vector<Producto>& inventario) {
    Producto nuevoProducto;
    cout << "\nIngrese los detalles del nuevo producto:\n";
    cout << "Nombre: ";
    cin.ignore();
    getline(cin, nuevoProducto.nombre);
    cout << "Codigo: ";
    cin >> nuevoProducto.codigo;
    cout << "Precio: Q";
    cin >> nuevoProducto.precio;
    cout << "Existencia: ";
    cin >> nuevoProducto.existencia;

    inventario.push_back(nuevoProducto);
    cout << "Producto agregado al inventario.\n";
}

int main() {
    vector<Producto> inventario = {{"manzanas", 1, 10.0, 5}, {"peras", 2, 15.0, 8}, {"bananos", 3, 10, 3}};

    Cliente cliente;
    cout << "Ingrese su nombre: ";
    cin.ignore();
    getline(cin, cliente.nombre);
    cout << "Ingrese su direccion: ";
    getline(cin, cliente.direccion);

    int opcion;
    do {
        cout << "\n1. Mostrar Inventario\n2. Agregar Producto al Inventario\n3. Comprar\n4. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarInventario(inventario);
                break;
            case 2:
                agregarProducto(inventario);
                break;
            case 3: {
                mostrarInventario(inventario);

                int codigoSeleccionado;
                cout << "\nIngrese el codigo del producto que desea comprar (0 para cancelar): ";
                cin >> codigoSeleccionado;

                if (codigoSeleccionado != 0) {
                    Producto productoSeleccionado = seleccionarProducto(inventario, codigoSeleccionado);

                    if (productoSeleccionado.nombre != "N/A") {
                        int cantidadCompra;
                        do {
                            cout << "Ingrese la cantidad que desea comprar (disponible: " << productoSeleccionado.existencia << "): ";
                            cin >> cantidadCompra;

                            if (cantidadCompra <= 0) {
                                cout << "Cantidad no valida. Ingrese una cantidad valida.\n";
                            } else if (cantidadCompra > productoSeleccionado.existencia) {
                                cout << "Error: La cantidad seleccionada es mayor a la existencia disponible. Ingrese una cantidad valida.\n";
                            }

                        } while (cantidadCompra <= 0 || cantidadCompra > productoSeleccionado.existencia);

                        // Actualizar existencia
                        for (auto& producto : inventario) {
                            if (producto.codigo == productoSeleccionado.codigo) {
                                producto.existencia -= cantidadCompra;
                                break;
                            }
                        }

                        // calcular precio total
                        double precioTotal = productoSeleccionado.precio * cantidadCompra;

                        // Generar factura
                        generarFactura(cliente, productoSeleccionado, cantidadCompra, precioTotal);

                        // Guardar datos 
                        guardarDatos(cliente, productoSeleccionado, cantidadCompra, precioTotal);

                        cout << "\nGracias por su compra\n";
                    } else {
                        cout << "Producto no encontrado o agotado. Compra cancelada.\n";
                    }
                } else {
                    cout << "Compra cancelada.\n";
                }
                break;
            }
            case 4:
                cout << "Saliendo del programa.\n";
                break;
            default:
                cout << "Opcion no válida. Inténtelo de nuevo.\n";
        }
    } while (opcion != 4);

 return 0;
}