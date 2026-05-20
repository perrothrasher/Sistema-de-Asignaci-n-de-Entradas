# Sistema de Gestión de Asignación de Entradas

Este proyecto es una aplicación de consola desarrollada en Lenguaje C diseñada para gestionar el proceso de asignación de entradas para un evento masivo, utilizando estructuras de datos dinámicas y persistencia en archivos de texto.

## Funcionalidades

El sistema automatiza la lectura de solicitudes, consolida datos por usuario y aplica reglas de negocio para la distribución de tickets:

1. Gestión Dinámica de Datos: Implementa una lista enlazada simple para almacenar y manipular la información de los clientes (RUT, nombre y cantidad de entradas) en tiempo real
2. Consolidación Automática: Al procesar el archivo de entrada, el programa identifica si un cliente ya existe mediante su RUT; si es así, suma las nuevas entradas solicitadas a su registro actual.
3. Lógica de Asignación: 
- Prioridad: Solicitudes de 1 a 2 entradas se asignan directamente hasta agotar el stock de 50 entradas.
- Restricción: A las solicitudes de 3 a 5 entradas se les asignan automáticamente solo 2 unidades.
- Filtrado: Solicitudes superiores a 5 entradas son descartadas del sistema.

4. Generación de Reportes: Crea archivos de salida personalizados basados en el nombre del archivo de entrada (.asg para asignados y .esp para lista de espera).


## Estructura Técnica
El código se organiza en funciones modulares para facilitar su mantenimiento:
- crearNodo: Asigna memoria RAM dinámicamente (malloc) para cada nuevo cliente.
- insertarFinal: Controla la inserción en la lista y la lógica de suma de entradas por RUT duplicado.
- analizarArchivo: Sistema de validación para asegurar que los archivos de entrada existan y sean legibles.
- eliminar: El motor principal que clasifica a los clientes en la lista de asignados o de espera según las reglas establecidas.

## Requisitos de los Datos de Entrada
El programa espera un archivo .txt con el siguiente formato por línea:

```bash
12333444-2,Maria Reyes,2
20165233-4,Rocio Morales,2
```
RUT,Nombre Completo,Cantidad_Entradas

## Ejecución

1. Compila el archivo utilizando un compilador de C (como GCC)
2. Ejecuta el programa e ingresa el nombre de tu archivo de datos (ej. datos1.txt) cuando se te solicite.
