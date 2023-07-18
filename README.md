# ESP32_VariadorPalomo

Controla la velocidad de un ventilador trifásico controlado por un Variador de Frecuencia a través de la entrada analógica, utilizando un ESP32. 

Cuenta con un sensor de temperatura para calcular la velocidad necesaria para mantener la temperatura en el recinto en un rango deseable.

Desarrollado en el entorno de platformio.


## Funcionalidades


### Conexión WIFI
El dispositivo funciona inicialmente como AP. A través de esta red se puede acceder a una página para configurar los parámetros de funcionamiento.

Se pueden configurar las credenciales de una red WIFI para que se conecte. Esto permite que sincronice la hora del dispositivo con un servidor ntp a elección.


### Perfiles de Funcionamiento

Desde la página web del dispositivo se pueden configurar 4 hasta perfiles de funcionamiento. Cada perfil tiene una curva de velocidad del husillo en función de la temperatura.
Estas curvas son funciones lineales de primer orden, definias como: 
```
vel(temp)= vel0 + rel * temp
```
A su vez estos perfiles se puden configurar para funcionar en un rango de horas diarias, indicando hora de inicio y hora de fin.
