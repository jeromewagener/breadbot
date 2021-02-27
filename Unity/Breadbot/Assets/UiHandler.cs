using UnityEngine;
using TMPro;

public class UiHandler : MonoBehaviour
{
    public GameObject serial;
    
    // Amount by how much the servo should move after clicking the up/down button
    public TMP_InputField servoSteps;
    // Amount by how much the stepper motors should move
    // For reference, 100 (straight) steps is about 30 centimeters or 12 inches in my case
    public TMP_InputField stepperSteps;
    
    // Default servo position. Same default as hardcoded in the Arduino code. (looking straight forward)
    private int _servoPosition = 70;
    
    public void MoveServoUp()
    {
        _servoPosition -= int.Parse(servoSteps.text);
        serial.GetComponent<SerialCom>().SendToArduino("moveServo=" + _servoPosition);
    }
    
    public void MoveServoDown()
    {
        _servoPosition += int.Parse(servoSteps.text);
        serial.GetComponent<SerialCom>().SendToArduino("moveServo=" + _servoPosition);
    }

    public void MoveForward()
    {
        serial.GetComponent<SerialCom>().SendToArduino("moveForward=" + stepperSteps.text);
    }

    public void MoveBackward()
    {
        serial.GetComponent<SerialCom>().SendToArduino("moveBackward=" + stepperSteps.text);
    }

    public void TurnLeft()
    {
        serial.GetComponent<SerialCom>().SendToArduino("turnLeft=" + stepperSteps.text);
    }

    public void TurnRight()
    {
        serial.GetComponent<SerialCom>().SendToArduino("turnRight=" + stepperSteps.text);
    }
}
