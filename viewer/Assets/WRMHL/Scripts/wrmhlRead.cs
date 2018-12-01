using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
This script is used to read all the data coming from the device. For instance,
If arduino send ->
								{"1",
								"2",
								"3",}
readQueue() will return ->
								"1", for the first call
								"2", for the second call
								"3", for the thirst call

This is the perfect script for integration that need to avoid data loose.
If you need speed and low latency take a look to wrmhlReadLatest.
*/

public class wrmhlRead : MonoBehaviour
{

    wrmhl myDevice = new wrmhl(); // wrmhl is the bridge beetwen your computer and hardware.

    [Tooltip("SerialPort of your device.")]
    public string portName = "COM3";

    [Tooltip("Match with Arduino Baudrate")]
    public int baudRate = 250000;


    [Tooltip("Make slightly higher than Arduino update frequency")]
    public int ReadTimeout = 205;

    public int QueueLength = 1;

    public Transform proxy;

    private string _data;
    private string[] _array;

    void Start()
    {
        myDevice.set(portName, baudRate, ReadTimeout, QueueLength);
        myDevice.connect();
    }

    void Update()
    {
        _data = myDevice.readQueue();

        if (_data != null)
        {
            _array = _data.Split(',');

            if (_array.Length == 4)
            {
                proxy.rotation = new Quaternion(
                    float.Parse(_array[0]),
                    float.Parse(_array[1]),
                    float.Parse(_array[2]),
                    float.Parse(_array[3])
                );
            }
        }
    }

    void OnApplicationQuit()
    {
        myDevice.close();
    }
}
