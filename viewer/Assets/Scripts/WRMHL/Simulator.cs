using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Simulator : MonoBehaviour
{
    private Quaternion[] rotation;
    private int[] jaw;
    private int step = 0;

    public float FPS = 20f;

    void Start()
    {
        string[] _lines = System.IO.File.ReadAllText("Assets/sample.CSV").Split("\n"[0]);

        string[] _x = _lines[0].Split(","[0]);
        string[] _y = _lines[1].Split(","[0]);
        string[] _z = _lines[2].Split(","[0]);
        string[] _w = _lines[3].Split(","[0]);
        string[] _jaw = _lines[4].Split(","[0]);

        rotation = new Quaternion[_x.Length];
        jaw = new int[_jaw.Length];

        int i = 0;
        foreach (var _ in rotation)
        {
            rotation.SetValue(
                new Quaternion(float.Parse(_x[i]), float.Parse(_y[i]), float.Parse(_z[i]), float.Parse(_w[i])),
                i++
            );
        }

        i = 0;
        foreach (var _ in _jaw)
        {
            jaw.SetValue(int.Parse(_), i++);
        }

        InvokeRepeating ("Render", 0f, 1f / FPS);
    }

    void Render()
    {
        Glove.rotation = rotation[step];
        Glove.jaw = jaw[step];

        step++;

        if (step >= jaw.Length) step = 0;
    }
}
