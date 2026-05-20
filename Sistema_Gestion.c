#include <stdio.h>
#include <stdlib.h>//libreria para listas
#include <string.h>
/*
ENTRADA: datos del cliente (rut, nombre y cantidad de entradas)
SALIDA: estructura de un nodo con los datos de cada cliente
PROCESO: esta es la estructura de la lista para los clientes que compren entradas para el el lollapalooza
dejando estructurados sus datos
*/
struct cliente{
    char rut[11];
    char nombre[50];
    int entradas;
    struct cliente*sig;
};
typedef struct cliente tCliente;
typedef tCliente *Lista;
char nombreArchivo[30]; //global
/*
ENTRADA: datos de los clientes que ingresaran a la lista (rut, nombre, cantidad de entradas)
SALIDA: nodos con los datos entregados por la funcion insertaFinal
PROCESO: la funcion creara nodos que tendran una estructura tCliente (linea 11). Declaramos que el puntero 
aux sea de tipo Lista, la funcion malloc es necesaria en este caso para asignar un espacio en la
memoria al nodo nuevo retornandonos un puntero hacia ese nodo. Para reemplazar un char es necesario
un strcpy, sin este retornara un error, para un entero o float no es necesario y el aux->sig del nodo
apuntara a NULL, retornando un aux apuntando un nodo.
*/
Lista crearNodo(char rut[11], char nombre [50], int entradas)
{
    Lista aux;
    aux= malloc(sizeof(tCliente));//asigna cantidad de memoria ram para los nodos(bytes)
    strcpy(aux->rut, rut);//strcpy: copiar string
    strcpy(aux->nombre, nombre);
    aux->entradas= entradas;
    aux->sig= NULL;
    return aux;
}
/*
ENTRADA: la Lista L, el rut, nombre y cantidad de entradas solicitadas por el cliente
SALIDA: entrega una lista ya modificada por la funcion leerArchivo y eliminar
PROCESO: esta funcion lo que hace es insertar al final de la Lista L un nuevo nodo con los datos que se
entregan. Declaramos que aux sea de tipo Lista, si el if es aplicado es porque la lista se encuentra vacia
y creara los nodos con la funcion creaNodo segun los datos entregados, si no es asi, es decir que la lista ya tenga nodos, entrara el
else, declarando que aux apunte a donde apunta L, ahora entra el while que recorrera toda la lista de los clientes, el strcmp va a comparar
el rut entregado con el rut a donde apunta el aux, si coinciden, se actualizara la cantidad de entradas del nodo para luego cambiar de nodo.
Terminando el while el aux apunta nuevamente a L y recorre nuevamente la lista creando los nodos con la cantidad de entradas ya 
actualizadas.
*/
Lista insertarFinal(Lista L, char rut[50], char nombre [50], int entradas)
{
    Lista aux;
    if (L==NULL) //en caso de no existir un nodo
    {
        L= crearNodo(rut, nombre, entradas);
    }
    else //si ya existen los nodos
    {
        aux= L;
        while(aux!=NULL)
        {
            if(strcmp(aux->rut, rut)==0)//si los rut son iguales entrega un 0 y entra al if
            {
                aux->entradas += entradas;//suma las entradas a las ya existentes por el +=
                return L;
            }
            aux= aux->sig;
        }
        aux= L;
        while(aux->sig!=NULL)
        {
            aux= aux->sig;
        }
        aux->sig= crearNodo(rut, nombre, entradas);
    }
    return L;
}
/*
ENTRADA: el archivo que se ingresa
SALIDA: si es ==NULL el archivo no pudo abrirse, si es != NULL el archivo pudo abrirse
PROCESO: si el puntero al archivo es nulo, significa que no puede abrir el archivo y cerrara el programa con la funcion exit, pero si el
puntero no es nulo, significa que puede abrir el archivo e ignorara las condiciones impuestas por el if de esta funcion. En simples palabras,
esta funcion solo se activa en caso de que el archivo no pueda abrirse.
*/
void analizarArchivo(FILE *archivo, char nombreArchivo[30])
{
    if (archivo == NULL)//nombre de archivo existente no es igual al ingresado por el usuario
    {
        printf("No es posible abrir el archivo %s.", nombreArchivo);
        printf("\n");
        system("pause");//presione una tecla para continuar
        exit(-1);
    }
}
/*
ENTRADA: Lista L escaneada desde el archivo .txt
SALIDA: Lista de clientes clasificados en: asignados y en caso de existir, la lista de espera. Crenando archivos de salida con estas listas
PROCESO: esta funcion crea 2 listas a partir de la lista L obtenida desde el archivo .txt, la lista de asignados y la lista de espera, recorriendo 
la lista L y comienza a buscar personas que cumplan con las condiciones: personas que han solicitado entre 1 y 2 entradas que entra automaticamente
a la lista de asignados y personas que han solicitado entre 3 y 5 entradas que entran a la lista de espera pero con 2 entradas solamente, el resto 
clientes (solicitan sobre 5 entradas) no entraran ni a lista de asignados ni de espera. Cuando se llena la lista de asignados (max. 50 entradas)
las personas que aun pueden ser aceptadas entran a la lista de espera. Despues a partir de estas listas la funcion se pregunta si existe la lista
de asignados creara el archivo con extension .asg que contendra la lista de personas asignadas. La lista de espera es opcional, si existe la lista
de espera, creara el archivo con extension .esp, en caso de no existir esta lista, no creara ningun tipo de archivo.
*/
void eliminar(Lista L)
{
    Lista aux;
    Lista ENTRADAS_ASIGNADAS;
    Lista ESPERA;
    int asignadas;
    int pendientes;

    asignadas= 0;
    pendientes= 50;
    ENTRADAS_ASIGNADAS= NULL;
    ESPERA= NULL;
    aux=L;
    while(aux!=NULL)
    {
        if(aux->entradas>=1 && aux->entradas <=2)//de 1 a 2 entradas
        {
            if(asignadas < 50) //if para entrar a la lista de asignados
            {
                ENTRADAS_ASIGNADAS= insertarFinal(ENTRADAS_ASIGNADAS, aux->rut, aux->nombre, aux->entradas);
                asignadas += aux->entradas;
                pendientes -= aux->entradas;
            }
            else //si no entra al if entra a la lista de espera
            {
                ESPERA= insertarFinal(ESPERA, aux->rut, aux->nombre, aux->entradas);
            }
        }
        if (aux->entradas >=3 && aux->entradas <=5)//de 3 a 5 entradas
        {
            aux->entradas =2;//las personas entre 3 a 5 entradas se les otorga 2 maximo
            ENTRADAS_ASIGNADAS= insertarFinal(ENTRADAS_ASIGNADAS, aux->rut, aux->nombre, aux->entradas);
            asignadas += aux->entradas;
            pendientes -= aux->entradas;
        }
        aux= aux->sig;
    }
    
    char nombreArchivoSalida_Asignadas[30];
    char nombreArchivoSalida_Espera[30];
    FILE*archSalida_Espera;
    FILE*archSalida_Asignadas;
    char rut[11];
    char nombre[50];
    int entradas;
    if(ENTRADAS_ASIGNADAS!=NULL)
    {
        strcpy(nombreArchivoSalida_Asignadas, nombreArchivo); //copia nombre de archivo de entrada para el de salida
        strtok(nombreArchivoSalida_Asignadas, "."); //lee el nombre hasta antes del .
        strcat(nombreArchivoSalida_Asignadas, ".asg");//le agrega la extension al archivo nuevo
        archSalida_Asignadas= fopen(nombreArchivoSalida_Asignadas, "w");//el w es para cuando no exista lo crea y si existe reescribe sobre el
        aux= ENTRADAS_ASIGNADAS;
        while(aux!=NULL)
        {
            fprintf(archSalida_Asignadas, "%s, %s, %i\n", aux->rut, aux->nombre, aux->entradas);//fprintf escribe dentro del archivo
            aux= aux->sig;
        }
        fprintf(archSalida_Asignadas, "Entradas Asignadas: %i\n", asignadas);
        fprintf(archSalida_Asignadas, "Entradas Pendientes: %i\n", pendientes);
        fclose(archSalida_Asignadas);
    } 
    if (ESPERA !=NULL)
    {
        strcpy(nombreArchivoSalida_Espera, nombreArchivo);
        strtok(nombreArchivoSalida_Espera, ".");
        strcat(nombreArchivoSalida_Espera, ".esp");
        archSalida_Espera= fopen(nombreArchivoSalida_Espera, "w");
        aux=ESPERA;
        while(aux!=NULL)
        {
            fprintf(archSalida_Espera, "%s, %s\n", aux->rut, aux->nombre);
            aux= aux->sig;
        }
        fclose(archSalida_Espera);
    }
}
/*
ENTRADA: archivo ingresado por el usuario
SALIDA: lee el archivo y le manda los datos a la funcion eliminar
PROCESO: primero se definen las variables y se definen punteros a otras, luego la funcion pide el nombre del archivo al usuario con el fgets,
leyendo el nombre del archivo ingresado por la entrada estandar (teclado). Despues entra el strtok que se encarga de eliminar el ENTER del final 
y con esto guardar el rut, nombre y la cantidad de entradas en sus respectivas variables y el fopen abrira el archivo con el modo "r" que este es
para leer. El archivo es analizado con el while y el fgets hasta encontrarse una linea en blanco. El strtok dividira la linea con los datos en 3 
secciones que son separados por la "," y el atoi retorna el 3er trozo como si fuese un entero. Se llama a la funcion insertaFinal para ir agregando 
nodos con la informacion ya modificada con los strtok (el rut, nombre y la cantidad de entradas) para mandarle la lista a la funcion imprimeLista.
El fclose cierra el archivo. Luego se llama a la funcion eliminar para modificar la lista L creada a partir del archivo .txt.
*/
void leerArchivo()
{
    char linea[100];
    FILE*archivo;
    char *rut;
    char *nombre;
    char *trozo;
    int entradas;
    Lista L;
    L=NULL;

    printf("Ingresar el nombre del archivo: ");
    fgets(nombreArchivo, 30, stdin);//pregunta en pantalla por el nombre del archivo, stdin= standard input
    strtok(nombreArchivo, "\n");//elimina el ENTER del final de linea
    archivo= fopen(nombreArchivo, "r");// con "r" leera el archivo
    analizarArchivo(archivo, nombreArchivo);//comparacion
    while(fgets(linea, 200, archivo)!=0)
    {
        if(strlen(linea)==1)
        {
            break;
        }
        strtok(linea, "\n");
        rut= strtok(linea, ",");
        nombre= strtok(NULL, ",");
        trozo= strtok(NULL, ",");
        entradas= atoi(trozo);//atoi en simples palabras "convierte" el string en un int
        L= insertarFinal(L, rut, nombre, entradas);
    }
    fclose(archivo);
    eliminar(L); //elimina a las personas que no cumplan con los requisitos
}
/*
ENTRADA: funcion leerArchivo
SALIDA: realiza la funcion leerArchivo
PROCESO: funcion principal, la primera en ejecutarse. En este caso ejecuta la funcion leerArchivo, esta ejecuta las demas funciones del algoritmo.
*/
int main()//funcion principal
{
    leerArchivo();//funcion que llamara a todas las demas
    printf("\n\n");
    system("pause");//pausa el programa pidiendo pulsar una tecla para continuar
    return 0;
}