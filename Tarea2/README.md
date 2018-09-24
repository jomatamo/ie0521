Jose Andres Matamoros Murillo
Carne B34093


**Simulador de cache parametrizable**
El siguiente programa es un simulador de cache parametrizable, donde se le puede
cambia el tamano del cache, el tamano de bloque y la asosiatividad. El simulador recibe un trace por el stardard input con todas las lineas de las direcciones de
memoria que se quieren accesar y el simulador se encarga de leerlas y empezar a
agregar los bloques respectivos de estas o a victimizalos si estan llenas la vias
para un index predertinado. El programa al final imprime la estasdiscticas de simulacion con el trace dado.

**Compilacion de codigo**
Para compilar el codigo se debe un utilizar en la consola:

  make
  PATH=$PATH:.

El segundo comando agrega el repositorio al path temporalemente, por lo que el nombre del ejecutable se cambio a cacheB34093

**Simulacion**
despues de ejecuado, se debe ejecutar el siguiente comando para realizar la simulacion:

  gunzip *trace comprimido* | cache34093 -t *tamano cache en bytes* -l *tamano del bloque* -a *numero de vias*
