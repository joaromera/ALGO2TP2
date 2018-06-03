#ifndef const_iterador_registros_h
#define const_iterador_registros_h

#include "Tabla.h"

/** @brief Iterador de los registros de una tabla */
class Tabla::const_iterador_registros {

public:
  /**
   * @brief Constructor por copia del iterador.
   *
   * \complexity{\O(1)}
   */
  const_iterador_registros(const const_iterador_registros& o_it);

  /**
   * @brief Desreferencia el puntero
   *
   * El valor devuelto tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es una referencia constante al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const Registro& operator*() const;
  
  /**
   * @brief Operador flechita
   *
   * El valor devuelvo tiene aliasing dentro de la colección.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post El valor resultado es un puntero al valor apuntado.
   *
   * \complexity{\O(1)}
   */
  const Registro *operator->() const;

  /**
   * @brief Avanza el iterador una posición.
   *
   * \pre El iterador no debe estar en la posición pasando-el-último.
   * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
   * siguiente.
   *
   * \complexity{\O(1)}
   */
  const_iterador_registros& operator++();

  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator==(const const_iterador_registros& o_it) const;
  
  /**
   * @brief Comparación entre iteradores 
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores no apuntan al mismo elemento
   *
   * \complexity{\O(1)}
   */
  bool operator!=(const const_iterador_registros& o_it) const;

private:
  friend class Tabla;

  const_iterador_registros(const linear_set<Registro>::const_iterator);

  linear_set<Registro>::const_iterator it_registro;

};

#endif // const_iterador_registros_h
