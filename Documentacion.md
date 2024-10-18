# Documentación para el código SerialCommunication
## Descripción general

Este código es un script de Unity en C# que maneja la comunicación serial entre una Raspberry Pi Pico (o cualquier otro dispositivo que use puertos COM) y una aplicación desarrollada en Unity. La comunicación se realiza a través del puerto serial, enviando y recibiendo comandos. También se incluyen funciones para controlar un LED, ajustar niveles de presión y nivel de agua, utilizando botones en la interfaz de usuario. Los datos recibidos se muestran en dos elementos de UI usando TextMeshProUGUI.

## Atributos de la clase
`SerialPort serialPort`: Instancia de la clase SerialPort, que permite abrir y gestionar la comunicación serial.

`string portName`: Nombre del puerto serial (por ejemplo, "COM7"). Debe cambiarse según el puerto en el que esté conectada la Raspberry Pi Pico.

`int baudRate`: Tasa de baudios (velocidad de comunicación) usada para la comunicación serial. En este caso, es 115200.

`TextMeshProUGUI _text`: Campo de texto donde se mostrará la información recibida.

`TextMeshProUGUI _text2`: Otro campo de texto para mostrar información adicional.

`Button encenderLedButton, apagarLedButton, subirPresionButton, bajarPresionButton, subirNivelButton, bajarNivelButton`: Referencias a los botones de la UI que activan diferentes comandos para controlar el dispositivo conectado.
## Métodos:

`void Start()`
- `Propósito`: Inicializa la comunicación serial y configura los botones de la UI.
- `Acciones`:
Inicializa el puerto serial con el nombre del puerto (portName) y la tasa de baudios (baudRate).
Intenta abrir el puerto serial y captura cualquier excepción en caso de fallo.
Asigna las funciones de cada botón mediante AddListener, para enviar comandos cuando se presionen.

`void Update()`
- `Propósito`: Lee y procesa los datos entrantes desde el puerto serial.
- `Acciones`:
Verifica si el puerto serial está abierto.
Si hay datos disponibles, los lee utilizando ReadLine y los procesa mediante la función ProcessReceivedData.

`void SendCommand(string command)`
- `Propósito`: Envía un comando al dispositivo conectado mediante el puerto serial.
- `Parámetro`: command - El comando que se enviará como una cadena de texto.
- `Acciones`:
Verifica que el puerto serial esté abierto.
Envía el comando al puerto y registra la operación en el Debug.Log.

- `Funciones de control`:
Cada una de estas funciones envía un comando específico al dispositivo cuando se pulsa un botón:

    - `EncenderLed()`: Envía el comando "EncenderLed".
    - `ApagarLed()`: Envía el comando "ApagarLed".
    - `SubirPresion()`: Envía el comando "SubirPresion".
    - `BajarPresion()`: Envía el comando "BajarPresion".
    - `SubirNivel()`: Envía el comando "SubirNivel".
    - `BajarNivel()`: Envía el comando "BajarNivel".

`void ProcessReceivedData(string data)`
- `Propósito`: Procesa los datos recibidos del puerto serial y actualiza los campos de texto de la UI.
- Parámetro: data - La cadena de texto recibida.
- `Acciones`:
Actualiza el componente _text con la información recibida.
También puede mostrar más información en _text2 si es necesario.

`void OnApplicationQuit()`
- `Propósito`: Cierra el puerto serial cuando la aplicación se cierra.
- `Acciones`:
Verifica si el puerto serial está abierto, y si es así, lo cierra.

### Ejemplo de uso:
Conectar la Raspberry Pi Pico al sistema a través de un puerto COM.
Configurar los botones de la UI en Unity para controlar el LED y otros parámetros.
Al ejecutar la aplicación, los botones enviarán comandos al dispositivo conectado, y los datos recibidos se mostrarán en pantalla.
Este código es útil para aplicaciones que necesitan interactuar con hardware externo a través de la comunicación serial, permitiendo un control fácil desde una interfaz gráfica en Unity.

# Documentación para el código ExtractNumbers
## Descripción general
Este script en Unity permite extraer números de un campo de texto utilizando expresiones regulares y asignar esos valores a variables correspondientes (temperatura, presión, nivel). Luego, el valor seleccionado por un menú desplegable (dropdown) se refleja en dos sliders de la interfaz. La función principal es procesar los números encontrados en el texto y actualizar los componentes de UI de acuerdo a la selección del usuario.

## Atributos de la clase
`int temperatura`: Variable que almacena el valor numérico extraído que corresponde a la temperatura.

`int presion`: Variable que almacena el valor numérico extraído que corresponde a la presión.

`int nivel`: Variable que almacena el valor numérico extraído que corresponde al nivel.

`TextMeshProUGUI respuesta`: Campo de texto que contiene los datos desde los cuales se extraen los números.

`Dropdown dropdown`: Menú desplegable que permite seleccionar qué valor (temperatura, presión o nivel) se mostrará en los sliders.

`Slider sliderNivel`: Slider que refleja el valor de nivel o el valor correspondiente según la selección en el dropdown.

`Slider sliderEstado`: Slider que refleja el estado de temperatura, presión o nivel dependiendo de la selección en el dropdown.

## Métodos
`void Cambiar()`
- `Propósito`: Actualiza los sliders (sliderEstado y sliderNivel) con los valores correspondientes (temperatura, presión o nivel) según la opción seleccionada en el dropdown.
- `Acciones`:
Lee el valor seleccionado en el dropdown (temperatura, presión o nivel).
Asigna el valor correspondiente a los sliders.

`void Contar()`
- `Propósito`: Extrae los números del campo de texto respuesta utilizando una expresión regular, y asigna los valores extraídos a las variables temperatura, presion, y nivel.
- `Acciones`:
Utiliza una expresión regular (\d+) para buscar números en el texto.
Itera sobre las coincidencias encontradas y asigna los primeros tres números a las variables de la clase (temperatura, presión y nivel), de acuerdo con el índice.
Depura los números encontrados mostrando los resultados en la consola de Unity con Debug.Log.

`void Update()`
- `Propósito`: Llama constantemente al método Contar para actualizar las variables basadas en los números del texto.
Nota: Este método se ejecuta en cada fotograma y puede ser optimizado para evitar que se llame a Contar innecesariamente en cada ciclo.

### Ejemplo de uso:
Se proporciona un texto en respuesta.text que contiene valores numéricos.
La función Contar extrae esos números del texto y los almacena en las variables correspondientes (temperatura, presión y nivel).
Dependiendo de la opción seleccionada en el dropdown, los sliders se actualizan con el valor seleccionado.

# Documentación para el código de control en Raspberry Pi Pico

## Descripción general
Este código controla un sistema de simulación de temperatura, presión y nivel de agua utilizando un Raspberry Pi Pico. El sistema se comunica con un proyecto de Unity a través de una conexión serial. Se utiliza una máquina de estados para gestionar los comandos enviados desde Unity, y las variables del sistema se actualizan y devuelven a través de la conexión serial.

## Variables globales
`unsigned long previousMillis`: Almacena el último tiempo registrado, utilizado para controlar el intervalo de actualización.

`const long interval = 3000`: Intervalo de tiempo en milisegundos (3 segundos) para la actualización de las variables.

`float contador = 60`: Contador que simula el tiempo o algún proceso que se va decrementando.

`float presion = 0`: Variable que representa la presión simulada.

`float nivel = 0`: Variable que representa el nivel de agua simulado.

`float intervalo = 0.1`: Incremento o decremento utilizado para ajustar los valores de las variables en respuesta a comandos.

`constexpr uint8_t ledPin = 25`: Pin del LED incorporado en la Raspberry Pi Pico, asignado a GPIO 25.

`float temperatura = 25.0`: Valor simulado de la temperatura inicial.

`float checksum = 0`: Variable utilizada para almacenar la suma de varios valores, potencialmente para verificación de integridad.

`float temperaturaEstado, presionEstado, nivelEstado`: Estados adicionales que almacenan el último valor de temperatura, presión y nivel.

## Máquina de estados
El sistema tiene dos estados principales que son gestionados por la máquina de estados:

`INIT`: Estado inicial donde el sistema espera comandos de configuración y ajustes de variables. Aquí es donde se reciben los comandos para modificar los valores de intervalo, contador, presion, y nivel. También se recibe el comando para iniciar el sistema.

`WAIT_COMMANDS`: Estado donde el sistema ejecuta la lógica principal. Cada 3 segundos (controlado por el intervalo interval), el sistema incrementa las variables simuladas (presión, nivel) y decrementa el contador, enviando los valores actualizados de vuelta al sistema de Unity. Además, permite recibir comandos adicionales para controlar el LED o reiniciar las variables.

## Funciones
`void task()`
- `Propósito`: Implementa la lógica de la máquina de estados. Cambia entre los estados INIT y WAIT_COMMANDS dependiendo de los comandos recibidos por la conexión serial.
- `Acciones`:
En el estado INIT, espera comandos desde Unity para modificar las variables (subirIntervalo, bajarIntervalo, SubirTiempo, BajarTiempo, SubirPresion, BajarPresion, SubirNivel, BajarNivel) y para cambiar al estado WAIT_COMMANDS con el comando "Iniciar".
En el estado WAIT_COMMANDS, cada 3 segundos, incrementa los valores de presión y nivel, y decrementa el contador. Envía estos valores a Unity y permite recibir comandos adicionales para controlar el LED o resetear las variables.

`void setup()`
- `Propósito`: Configura el entorno inicial de la Raspberry Pi Pico.
- `Acciones`:
Inicializa la comunicación serial a una velocidad de 115200 baudios.
Configura el pin GPIO 25 (el LED incorporado) como salida.

`void loop()`
- `Propósito`: Ejecuta la máquina de estados constantemente dentro del bucle principal del microcontrolador.
- `Acciones`:
Llama a la función task() en cada ciclo del bucle principal.
Comandos seriales
El sistema responde a los siguientes comandos enviados desde Unity:

    - subirIntervalo / bajarIntervalo: Aumenta o disminuye el valor del intervalo en 0.1 unidades, dentro del rango permitido (0.1 a 0.9).
    - SubirTiempo / BajarTiempo: Aumenta o disminuye el valor del contador, respetando el límite máximo de 60.
    - SubirPresion / BajarPresion: Aumenta o disminuye la presión simulada, con un valor máximo de 25.
    - SubirNivel / BajarNivel: Aumenta o disminuye el nivel de agua simulado, con un valor máximo de 25.
    - Iniciar: Cambia al estado WAIT_COMMANDS y apaga el LED.
    - C1234: Reinicia los valores de temperatura, presion y nivel a 0.
    - LEDOn / LEDOff: Enciende o apaga el LED.

### Ejemplo de uso
Desde Unity, se pueden enviar comandos para modificar el intervalo y controlar el temporizador, la presión y el nivel.
El sistema responde modificando los valores simulados y enviando los resultados de vuelta a Unity.
El LED incorporado puede ser controlado remotamente para encenderse o apagarse.
