#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

using namespace std;

// Estructura principal con nombres descriptivos
struct Producto {
    string codigo;
    string nombre;
    int cantidad;
    double precio;
};


// FUNCIONES DE AYUDA


// Retorna el indice del producto si existe, o -1 si no se encuentra.
// Centraliza la busqueda para reutilizarla en Agregar, Actualizar y Vender.
int BuscarProducto(const vector<Producto>& inventario, const string& codigo) {
    for (size_t i = 0; i < inventario.size(); ++i) {
        if (inventario[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// MÓDULOS DE RESPONSABILIDAD ÚNICA

void MostrarMenu() {
    printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("      GadgetXpress - Inventario\n");
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("1. Agregar nuevo producto\n");
    printf("2. Listar todos los productos\n");
    printf("3. Actualizar cantidad de un producto\n");
    printf("4. Generar reporte de bajo inventario\n");
    printf("5. Simular venta de producto\n");
    printf("6. Salir\n");
    printf("Seleccione una opcion: ");
}

void AgregarProducto(vector<Producto>& inventario) {
    char buffer_codigo[50];
    printf("\n--- Agregar Producto ---\n");
    printf("Ingrese el codigo: ");
    scanf("%49s", buffer_codigo);
    string codigo = buffer_codigo;

    if (BuscarProducto(inventario, codigo) != -1) {
        printf("[Error] Ya existe un producto con el codigo '%s'.\n", codigo.c_str());
        return;
    }

    Producto nuevo_producto;
    nuevo_producto.codigo = codigo;

    // Limpiamos el buffer residual de scanf antes de leer cadenas con espacios
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    char buffer_nombre[100];
    printf("Ingrese el nombre: ");
    fgets(buffer_nombre, sizeof(buffer_nombre), stdin);
    
    // Eliminamos el salto de linea automatico generado por fgets
    size_t longitud = strlen(buffer_nombre);
    if (longitud > 0 && buffer_nombre[longitud - 1] == '\n') {
        buffer_nombre[longitud - 1] = '\0';
    }
    nuevo_producto.nombre = buffer_nombre;

    printf("Ingrese la cantidad inicial: ");
    scanf("%d", &nuevo_producto.cantidad);

    printf("Ingrese el precio: ");
    scanf("%lf", &nuevo_producto.precio);

    inventario.push_back(nuevo_producto);
    printf("[Exito] Producto agregado correctamente.\n");
}

void ListarProductos(const vector<Producto>& inventario) {
    if (inventario.empty()) {
        printf("\n[Info] El inventario esta vacio.\n");
        return;
    }

    printf("\n--- Inventario Completo ---\n");
    printf("%-10s %-25s %-10s %-10s\n", "CODIGO", "NOMBRE", "CANTIDAD", "PRECIO");
    printf("---------------------------------------------------------\n");

    for (size_t i = 0; i < inventario.size(); ++i) {
        printf("%-10s %-25s %-10d $%.2f\n", 
               inventario[i].codigo.c_str(), 
               inventario[i].nombre.c_str(), 
               inventario[i].cantidad, 
               inventario[i].precio);
    }
}

void ActualizarCantidad(vector<Producto>& inventario) {
    char buffer_codigo[50];
    printf("\n--- Actualizar Stock ---\n");
    printf("Ingrese el codigo del producto: ");
    scanf("%49s", buffer_codigo);
    string codigo = buffer_codigo;

    int indice = BuscarProducto(inventario, codigo);

    if (indice == -1) {
        printf("[Error] El producto no existe en el inventario.\n");
        return;
    }

    int nueva_cantidad;
    printf("Producto actual: %s (Stock: %d)\n", inventario[indice].nombre.c_str(), inventario[indice].cantidad);
    printf("Ingrese la nueva cantidad total: ");
    scanf("%d", &nueva_cantidad);

    if (nueva_cantidad < 0) {
        printf("[Error] La cantidad no puede ser negativa.\n");
    } else {
        inventario[indice].cantidad = nueva_cantidad;
        printf("[Exito] Stock actualizado correctamente.\n");
    }
}

void GenerarReporteBajoInventario(const vector<Producto>& inventario) {
    const int umbral_critico = 5;
    bool encontrados = false;

    printf("\n--- Reporte: Bajo Inventario (Menos de %d unidades) ---\n", umbral_critico);

    for (size_t i = 0; i < inventario.size(); ++i) {
        if (inventario[i].cantidad < umbral_critico) {
            printf("- %s (Codigo: %s) | Quedan: %d unidades.\n", 
                   inventario[i].nombre.c_str(), 
                   inventario[i].codigo.c_str(), 
                   inventario[i].cantidad);
            encontrados = true;
        }
    }

    if (!encontrados) {
        printf("[Info] Todos los productos tienen un stock saludable.\n");
    }
}

// NUEVA FUNCIÓN: Simulación de ventas integrando validaciones robustas
void SimularVenta(vector<Producto>& inventario) {
    char buffer_codigo[50];
    printf("\n--- Simular Venta ---\n");
    printf("Ingrese el codigo del producto a vender: ");
    scanf("%49s", buffer_codigo);
    string codigo = buffer_codigo;

    // Validación 1: Verificar existencia reutilizando funcion de ayuda
    int indice = BuscarProducto(inventario, codigo);

    if (indice == -1) {
        printf("[Error] Producto no encontrado. Verifique el codigo ingresado.\n");
        return;
    }

    int cantidad_a_vender;
    printf("Producto seleccionado: %s | Stock disponible: %d\n", 
           inventario[indice].nombre.c_str(), inventario[indice].cantidad);
    printf("Ingrese la cantidad que desea vender: ");
    scanf("%d", &cantidad_a_vender);

    if (cantidad_a_vender <= 0) {
        printf("[Error] La cantidad a vender debe ser un numero positivo.\n");
        return;
    }

    // Validación 2: Verificar suficiencia de stock
    if (cantidad_a_vender > inventario[indice].cantidad) {
        printf("[Error] Inventario insuficiente. Solo dispone de %d unidades.\n", inventario[indice].cantidad);
    } else {
        // Ejecución de la venta (disminución lógica del inventario)
        inventario[indice].cantidad -= cantidad_a_vender;
        printf("[Exito] Venta realizada. El nuevo stock de '%s' es %d unidades.\n", 
               inventario[indice].nombre.c_str(), inventario[indice].cantidad);
    }
}


// BLOQUE PRINCIPAL


int main() {
    vector<Producto> inventario;
    int opcion;

    do {
        MostrarMenu();
        
        // Validacion de entrada para evitar bucles infinitos si se ingresa un caracter
        if (scanf("%d", &opcion) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcion = 0;
        }

        switch (opcion) {
            case 1: AgregarProducto(inventario); break;
            case 2: ListarProductos(inventario); break;
            case 3: ActualizarCantidad(inventario); break;
            case 4: GenerarReporteBajoInventario(inventario); break;
            case 5: SimularVenta(inventario); break;
            case 6: printf("\nCerrando sistema de GadgetXpress. Hasta pronto.\n"); break;
            default: printf("\n[Error] Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 6);

    return 0;
}