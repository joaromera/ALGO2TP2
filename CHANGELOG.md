## 1.1

* Actualización abs BaseDeDatos
* Actualización interfaz join en enunciado
* Actualización interfaz Tabla
* CMakeLists tiene dos targets:
    * `correrTests` ahora corre los tests pre solución (compila y
      corre). Esto funciona con el tp vainilla.
    * `correrTests_sol` trata de compilar y correr los tests post solución. 
      Esto NO funciona con el tp vainilla.

## 1.2

* Usos de fast_insert de los linear_map/set donde corresponde.
* Actualización complejidad constructor Registro
* Aclaraciones complejidad y post en linear_set y linear_map
* Actualización rep de linear_map para que refleje que no hay repetidos
* Actualización descripción de insert en string_map
* Correcciones menores de enunciado
* Corrección de restricción de combinarRegistros
* Corrección de CMakeLists.txt

## 1.2.1

* Corrección en linear_set_test por mal uso de la función copy.

## 1.3

* Tests de strings sin ñ
* String map tienen en cuenta el costo de copiar y comparar T
* Detalle del tests de join
* Aclaración en la complejidad del iterador de string_map
