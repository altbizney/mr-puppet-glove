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
    public static int jaw = 0;

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
            if (_data.StartsWith("DEBUG"))
            {
                Debug.Log(_data);
            }
            else
            {
                //Debug.Log(_data);

                _array = _data.Split(',');

                if (_array[0] == "E")
                {
                    // euler + flex
                    // TODO: understand the axis manipulation here better
                    rotation = Quaternion.Euler(
                        float.Parse(_array[2]) * -1,
                        float.Parse(_array[1]),
                        float.Parse(_array[3])
                    );
                    jaw = int.Parse(_array[4]);
                }
                else if (_array[0] == "Q")
                {
                    // quat + flex
                    // TODO: determine if this needs axis manipulation also
                    rotation = new Quaternion(
                        float.Parse(_array[1]),
                        float.Parse(_array[2]),
                        float.Parse(_array[3]),
                        float.Parse(_array[4])
                    );

                    jaw = int.Parse(_array[5]);
                }
            }

            // DebugGraph.MultiLog(rotation.x, "x");
            // DebugGraph.MultiLog(rotation.y, "y");
            // DebugGraph.MultiLog(rotation.z, "z");
            // DebugGraph.MultiLog(rotation.w, "w");
            // DebugGraph.MultiLog(jaw, "jaw");
        }
    }

    void OnApplicationQuit()
    {
        try
        {
            bridge.close();
        }
        catch { }
    }

    void OnDrawGizmos()
    {
        Debug.DrawRay(Vector3.zero, rotation * transform.forward, Color.red, 0f, true);
        Debug.DrawRay(Vector3.zero, rotation * transform.up, Color.green, 0f, true);
        Debug.DrawRay(Vector3.zero, rotation * transform.right, Color.blue, 0f, true);

    }
}
