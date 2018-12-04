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

    private float flex = 0.0f;
    public float flexMin = 9200.0f;
    public float flexMax = 35000.0f;

    public Transform orient;
    public Transform root;
    public Transform joint;

    private string _data;
    private string[] _array;
    private float _flex = 0f;
    private Quaternion _rotationQ;
    private Vector3 _rotationE;

    void Start()
    {
        myDevice.set(portName, baudRate, ReadTimeout, QueueLength);
        myDevice.connect();
    }

    void FixedUpdate()
    {
        _data = myDevice.readQueue();

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
                    root.localEulerAngles = new Vector3(
                        float.Parse(_array[1]) * -1,
                        float.Parse(_array[0]),
                        float.Parse(_array[2])
                    );
                    _flex = float.Parse(_array[3]);
                }
                else if (_array.Length == 5)
                {
                    // quat + flex
                    root.localRotation = new Quaternion(
                        float.Parse(_array[0]),
                        float.Parse(_array[1]),
                        float.Parse(_array[2]),
                        float.Parse(_array[3])
                    );

                    _flex = float.Parse(_array[4]);
                }

                flex = Mathf.InverseLerp(flexMin, flexMax, _flex);
                joint.localEulerAngles = new Vector3(Mathf.Lerp(0f, -50f, flex), 0f, 0f);
            }
        }
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            flexMax = _flex;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            flexMin = _flex;
        }
        else if (Input.GetKeyDown(KeyCode.Space))
        {
            orient.rotation = Quaternion.Inverse(root.localRotation);
        }
    }

    void OnApplicationQuit()
    {
        myDevice.close();
    }
}
