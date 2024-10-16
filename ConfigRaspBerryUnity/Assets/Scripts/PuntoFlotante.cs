using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using TMPro;
using UnityEngine;
using UnityEngine.Rendering.Universal;
using UnityEngine.UI;

enum Estado
{
    INIT,
    WAIT_COMMANDS,
    FINAL
}

public class PuntoFlotante : MonoBehaviour
{
    [SerializeField] TextMeshProUGUI _text;
    [SerializeField] TextMeshProUGUI _text2;
    private static Estado taskState = Estado.INIT;
    private SerialPort _serialPort;
    private byte[] buffer;
    byte[] bytes;
    string temp = "";

    void Start()
    {
        _serialPort = new SerialPort();
        _serialPort.PortName = "COM4";  // Cambia el puerto según sea necesario
        _serialPort.BaudRate = 115200;
        _serialPort.DtrEnable = true;
        _serialPort.NewLine = "\n";
        _serialPort.Open();
        Debug.Log("Open Serial Port");
        buffer = new byte[128];
    }

    void Update()
    {
        StartCoroutine(EstadoMaquina());
    }

    IEnumerator EstadoMaquina()
    {
        yield return new WaitForSeconds(1);  // Añadido un pequeño retraso para la estabilidad
        switch (taskState)
        {
            case Estado.INIT:
                if (Input.GetKeyDown(KeyCode.Return))
                {
                    Debug.Log("Enter");
                    _serialPort.Write("Inicio\n");
                    taskState = Estado.WAIT_COMMANDS;
                }
                break;

            case Estado.WAIT_COMMANDS:
                if (Input.GetKeyDown(KeyCode.Return))
                {
                    _serialPort.Write("Transmitir\n");
                    yield return new WaitForSeconds(0.2f);  // Espera para asegurar que el comando se procesa
                }
                if (Input.GetKeyDown(KeyCode.A))
                {
                    _serialPort.Write("LEDOff\n");
                    yield return new WaitForSeconds(0.2f);  // Espera para asegurar que el comando se procesa
                }

                if (_serialPort.BytesToRead >= 4)
                {
                    _serialPort.Read(buffer, 0, 4);
                    temp = "";  // Limpiamos la variable 'temp' para evitar que se concatene
                    for (int i = 0; i < 4; i++)
                    {
                        temp += buffer[i].ToString("X2");
                    }

                    uint num = uint.Parse(temp, System.Globalization.NumberStyles.HexNumber);
                    bytes = BitConverter.GetBytes(num);
                    Debug.Log(BitConverter.ToSingle(bytes, 0));

                    // Actualizamos el texto en la UI
                    _text.text = temp;
                    _text2.text = BitConverter.ToSingle(bytes, 0).ToString();
                }
                break;

            case Estado.FINAL:
                taskState = Estado.INIT;
                break;

            default:
                Debug.Log("State Error");
                break;
        }
    }
}
