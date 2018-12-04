using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenJaw : MonoBehaviour
{
    public float flexMin = 9200.0f;
    public float flexMax = 35000.0f;

    public Transform joint;

    void Update()
    {
        joint.localEulerAngles = new Vector3(
            Mathf.Lerp(
                0f,
                -50f,
                Mathf.InverseLerp(flexMin, flexMax, wrmhlRead.flex)
            ),
            0f,
            0f
        );

        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            flexMax = wrmhlRead.flex;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            flexMin = wrmhlRead.flex;
        }
    }
}
