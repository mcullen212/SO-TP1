# SO - TP1
Magdalena Cullen - Timoteo Feeney - Valentino Sanguinetti

Este trabajo práctico tiene como objetivo implementar un sistema que calcule el md5 de múltiples archivos de forma distribuida en varios procesos usando los distintos tipos de IPCs presentes en un sistema POSIX. Para la comunicación entre el proceso Master y los procesos slave, hemos optado por utilizar dos pipes por cada slave, lo cual nos permite manejar la entrada y salida de datos entre ambos, de manera unidireccional. Además, se implementó una memoria compartida para intercambiar y gestionar la información entre la aplicación principal (app) y el proceso de visualización (view), asegurando una sincronización entre estos componentes.

## Instrucciones de compilación y ejecución
### Compilación
Para compilar, ejecutar `make all`:
``` 
make all
```

### Ejecución - proceso aplicación con vista
Para ejecutar el proceso app vinculada directamente con la view:
```
./app <files> | ./view
```

### Ejecución - proceso aplicación sin vista
Para ejecutar el proceso app solo, y luego optar por correrlo la view también:
```
./app <files>
```
Y luego si quieren correr el view en otra terminal, donde shm_name es el nombre que devolvió el app:
```
./view <shm_name>
```

### Opcional: Creación de archivos testFiles para probar
Contamos con una carpeta `testFiles` en la cual se puede optar por correr `createFiles.sh`, y cortar la ejecución cuando se determina que se creraron suficientes archivos de prueba. Pueden llegar a tener que darle acceso con `chmod u+x`:
```
./createFiles.sh
```
Y luego para cortar la ejecución:
```
cntrl + C
```