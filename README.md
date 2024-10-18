# Unidad3UI

+-----------------------------------+
|           SerialCommunication     |
+-----------------------------------+
| - serialPort: SerialPort          |
| - portName: string                |
| - baudRate: int                   |
| - _text: TextMeshProUGUI          |
| - _text2: TextMeshProUGUI          |
| - encenderLedButton: Button       |
| - apagarLedButton: Button         |
| - subirPresionButton: Button      |
| - bajarPresionButton: Button      |
| - subirNivelButton: Button        |
| - bajarNivelButton: Button        |
+-----------------------------------+
| + Start()                         |
| + Update()                        |
| + EncenderLed()                   |
| + ApagarLed()                     |
| + SubirPresion()                  |
| + BajarPresion()                  |
| + SubirNivel()                    |
| + BajarNivel()                    |
| + ProcessReceivedData(data: string)|
| + OnApplicationQuit()             |
+-----------------------------------+
                |
                v
+-----------------------------------+
|          SerialPort (Lib)         |
+-----------------------------------+
| - WriteLine(command: string)      |
| - ReadLine(): string              |
+-----------------------------------+

          Unity                   Raspberry Pi Pico
           |                             |
           v                             v
 +----------------+           +--------------------------+
 |  SendCommand() |           |     taskState (Enum)      |
 |  ProcessReceivedData()      | + INIT                   |
 |  Button Control             | + WAIT_COMMANDS          |
 +----------------+            +--------------------------+
           |                              |
           v                              |
 +--------------------------+    +----------------------------+
 |      Serial Communication|    |      task()                 |
 |      over USB (COM Port)  |    | - INIT state handling       |
 +--------------------------+    | - WAIT_COMMANDS state       |
                                  +----------------------------+
                                  | + setup()                   |
                                  | + loop()                    |
                                  | + LED Control               |
                                  +----------------------------+

