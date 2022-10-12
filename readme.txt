Breve explicación del funcionamiento del programa.

Opté por utilizar una tabla hash para guardar el diccionario. Inicialmente tiene una capacidad de 1000 elementos, y a
medida que alcanza un factor de carga de 0.6, se va redmiensionando al doble de su capacidad. Para solucionar las 
colisiones, utiliza encadenamiento con listas enlazadas simples.

Una vez guardado el diccionario en la tabla, comienza a recorrer el archivo de entrada tomado como argumento del 
programa. Por cada palabra verifica que esté en el diccionario, si no está busca hasta 5 sugerencias utilizando las 
técnicas dadas. 
Por cada distancia busca sugerencias siguiendo el siguiente orden: interecambiar - eliminar - reemplazar - insertar. 
De esta forma, estan de menor a mayor según la cantidad de operaciones que tiene que hacer cada una.
Por último intenta separando la palabra, en este paso no guarda las sugerencias erróneas, sin embargo en las otras sí
lo hace, y como estructura decidí usar tabla hash.

Si en la primera distancia no encuentra 5 sugerencias, toma la tabla hash de las sugerencias erróneas obtenidas, las 
copia en un arreglo de strings que luego utiliza para recorrerlo y buscar sugerencias suyas, sin tocar la tabla hash
anterior. Si se recorriera directamente la tabla, como se va redimensionando, se modificaría el orden de las palabras,
produciendo que una misma sugerencia se pueda utilizar más de una vez o directamente no se utilice. 
Al terminar esta distancia, elimina el arreglo.

Si luego de la segunda distancia tampoco se obtuvieron 5 sugerencias de la palabra original, se buscan sugerencias de las
palabras erróneas guardadas en la tabla hash en la segunda distancia. No se copian en un arreglo aparte ya que las 
sugerencias erróneas que se obtienen en la tercera distancia no se guardan por lo que la tabla no se redimensiona.

Las sugerencias válidas que se encuentran se van guardando en una estructura junto a la palabra original,
la cantidad de sugerencias y la linea en la que se leyó la palabra en el texto de entrada.
Esta estructura se guarda en una lista simple general (la misma que se utiliza para las colisiones de las tablas).

Una vez que se recorrió todo el texto, se cierra y se recorre la lista con todas las estructuras (SugValidas) imprimiendo
en el archivo de salida toda la información obtenida (palabras erróneas, sugerencias y línea en la que se encontraron).
