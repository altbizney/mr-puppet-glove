﻿using System.Collections;
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

public class Glove : MonoBehaviour
{

    private wrmhl bridge = new wrmhl();

    [Tooltip("SerialPort of your device.")]
    public string portName = "COM3";

    [Tooltip("Match with Arduino Baudrate")]
    public int baudRate = 250000;

    [Tooltip("Make slightly higher than Arduino update frequency")]
    public int ReadTimeout = 205;

    public int QueueLength = 1;

    public static Quaternion rotation;
    public static float flex = 0f;

    private string _data;
    private string[] _array;

    void Start()
    {
        bridge.set(portName, baudRate, ReadTimeout, QueueLength);
        bridge.connect();
    }

    void Update()
    {
        _data = bridge.readQueue();

        if (_data != null)
        {
            if (_data.StartsWith("DEBUG;"))
            {
                Debug.Log(_data);
            }
            else
            {
                // Debug.Log(_data);

                _array = _data.Split(',');

                if (_array.Length == 4)
                {
                    // euler + flex
                    // TODO: understand the axis manipulation here better
                    rotation = Quaternion.Euler(
                        float.Parse(_array[1]) * -1,
                        float.Parse(_array[0]),
                        float.Parse(_array[2])
                    );
                    flex = float.Parse(_array[3]);
                }
                else if (_array.Length == 5)
                {
                    // quat + flex
                    // TODO: determine if this needs axis manipulation also
                    rotation = new Quaternion(
                        float.Parse(_array[0]),
                        float.Parse(_array[1]),
                        float.Parse(_array[2]),
                        float.Parse(_array[3])
                    );

                    flex = float.Parse(_array[4]);
                }
            }
        }
    }

    void OnApplicationQuit()
    {
        bridge.close();
    }
}