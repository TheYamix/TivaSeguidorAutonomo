# TivaSeguidorAutonomo
Carro autonomo y seguidor de linea usando tiva c launchpad
## Robot Autonomo -  Seguidor de Línea

![](https://i.postimg.cc/QCy0BV73/foto-seguidor-de-linea.png)

> Integrantes:
- Yamil Ivan Nuñez Alvarez
- Jose Velasquez VegasJose Velasquez Vegas

> Asesor:
- Francisco Segovia


------------

### Descripcion del proyecto:

El presente proyecto a implementar consiste en armar un vehículo controlado de manera remota que este tenga la función de seguir una línea negra y adicionalmente este se pueda manejar mediante comandos ingresados por el usuario. Para ello, este se ayudará de 2 sensores infrarrojos, 2 módulos bluetooth, cables para protoboard, una maqueta con 2 motores de 6v y un driver de motor.
El programa para el funcionamiento del robot fue diseñado en lenguaje c++ y ejecutado por keil u vision.
El microcontrolador que se usará es el EK-TM4C123GXL Evaluation board.

> El archivo Modulo Maestro contiene el modulo del Maestro
> El archivo PROYECTO_01_2022 contiene el modulo del Esclavo
> Los requerimientos se podrán observar en requerimientos.txt (subido al repositorio)
> El diagrama de flujo esta en formato PDF en 
Link:
`<https://www.ti.com/tool/EK-TM4C123GXL](https://www.ti.com/tool/EK-TM4C123GXL>` 

------------


### Especificaciones de Componentes: 

![](https://i.postimg.cc/mZXD6X5P/hc-05.png)
![](https://i.postimg.cc/VsbWWVgF/tcrt5000.png)
![](https://i.postimg.cc/PrS7BcG8/tb66.png)
![](https://i.postimg.cc/rpTqfmv9/motor-6v.png)

------------


### Diseño de Hardware

##### 1. Diagrama de Bloques

El proyecto consta de 2 microcontroladores TM4C123GH6PM. Uno de estos es conectado a una pc que funciona como periférico de entrada  para que mande las instrucciones al segundo, el cual recibirá las instrucciones y las realizará. Para la comunicación entre estos 2 microcontroladores se usarán 2 módulos bluetooth. Adicionalmente este segundo microcontrolador estará conectado a un sensor IMU que nos proporcionará información sobre la aceleración que tenga el vehículo, un par de sensores infrarrojos para analizar si se detectó luz o no  y un driver de motor dual para conectar los motores y regular su sentido de giro y revoluciones según el primer microcontrolador lo solicite.
Usaremos los puertos de proposito general para conectar los componentes. Adicionalmente para la comunicación entre ellos se usarán los protocolos UART y para controlar las revoluciones de giro del motor se configurará el Timer y PWM.

![](https://i.postimg.cc/RF7RBPFt/digrama-1.png)

------------


##### 2. Diagrama de Esquematico

- Modulo Maestro

![](https://i.postimg.cc/v8zm1Wxt/Diagrama-en-blanco-15.png)

> Las conexiones están explicadas en Información sobre conexiones.txt
(subido al repositorio)


- Modulo Esclavo

![](https://i.postimg.cc/nhKSZcPr/Diagrama-en-blanco-16.png)

> Las conexiones están explicadas en Información sobre conexiones.txt 
(subido al repositorio)

##### 3.Diagrama de Flujo de Datos

![](https://i.postimg.cc/W12pst1f/flujo-de-datos-1.png)

![](https://i.postimg.cc/yYS3DLc3/flujo-de-datos-2.png)


------------

### Observaciones y/o Recomendaciones:

1. Se recomienda preparar previamente un cronograma de tareas mediante un diagrama de gantt y/o plataformas digitales como por ejemplo: Trello.
2.  Al momento de configurar el  módulo bluetooth, tuvimos inconvenientes para hacer la vinculación entre bluetooth maestro y esclavo mediante el KEIL, es por ello que se recomienda hacer la vinculación y las diferentes configuraciones usando un arduino, en nuestro caso usamos un arduino UNO.
3. Cuando se hagan las configuraciones del bluetooth , se recomienda entrar en el modo “AT”y poner el estado de reseteo de fábrica, y luego empezar la configuración según lo que se requiera. Esto para evitar cruzarnos con algunas configuraciones no deseadas.
4. Un modulo bluetooth se debe configurar cómo maestro y este debe tener la direccion del otro bluetooth para que estos se conecten directamente. Cabe seraltar que para el modo AT el bluetooth HC-05 funciona a 38400 bps.
5. Se recomienda revisar detalladamente los datasheet de cada componente para estar seguro de las conexiones y evitar daños. Además, también se debe revisar si hay continuidad en los cables que se usará para las conexiones, en este proyecto en varias ocasiones tuvimos inconvenientes con los cables ya que estaban dañados internamente.
6. Debido a la escasa información sobre el motor, se hizo los calculos del angulo de giro de forma experimental para asi tener una referencia de cuanto debia girar en el seguidor de linea. En nuestro caso aproximamos a unos 5 grados.
7. Durante el momento de las pruebas tener baterias de repuesto, ya que la mayoria de los componentes están conectadas a una unica fuente de 4.5V.

------------


### Conclusiones:
- Las restricciones para este proyecto son las siguientes: -Restricciones propias del curso: Empleo del microcontrolador TM4C123GH6PM.
- Uso de Lenguaje C para la programación Entorno Keil uVision
- Restricciones específicas del proyecto: Empleo de interfaces UART, TIMER para la creación de señales PWM las cuales nos ayudarán con el ancho de banda de la señal.
- Gracias a la comunicación UART, I2C y el uso del TIMER en el TIVA TM4C123GH6PM se pudo realizar este proyecto con éxito.


------------


### Resultados:

Fotos de conexiones y ensamblado

![](https://i.postimg.cc/Z5950nNb/resultado1.png)

![](https://i.postimg.cc/pd4GxGHV/resultado2.png)

Fotos de simulacion del programa en PuTTY

![](https://i.postimg.cc/Z5gFyWDV/Whats-App-Image-2022-11-21-at-6-59-40-PM-1.jpg)
![](https://i.postimg.cc/jj766RpK/Whats-App-Image-2022-11-21-at-6-59-41-PM.jpg)

### End
