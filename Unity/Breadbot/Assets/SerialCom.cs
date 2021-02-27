using System;
using UnityEngine;
using System.IO.Ports;
using TMPro;

public class SerialCom : MonoBehaviour
{
    // Door to the magic world of serial communication
    private SerialPort _serialStream;
    
    // Cache of the last status we received from the Arduino
    // This is needed as the robot is not sending updates in all cases
    private String _status = "";
    
    // The status label on the UI
    public TMP_Text statusText;
    
    void Start()
    {
        // Good old SerialPort communication with the Arduino
        // Works the same as the Serial Monitor in the Arduino Studio
        _serialStream = new SerialPort("COM3", 9600) {ReadTimeout = 50};
        _serialStream.Open();
        
        // Asynchronous repeated status reading from the Arduino
        // Needed to avoid blocking the UI (e.g. webcam image) during reads etc...
        InvokeRepeating(nameof(ReadStatusFromArduino), 0, 2f);
    }

    public void SendToArduino(String message)
    {
        // Sends a string to the Arduino
        _serialStream.WriteLine(message);
        _serialStream.BaseStream.Flush();
    }

    public void ReadStatusFromArduino()
    {
        // If we cannot read anything after 200 milliseconds, we timeout...
        _serialStream.ReadTimeout = 200;
        try
        {
            _status = _serialStream.ReadLine();
        }
        catch (TimeoutException e)
        {
            // ... and end up in here
            // Here be dragons...
        }

        // If we were able to read something from the Arduino then we also display it
        statusText.text = "Status: " + _status;
    }
}
