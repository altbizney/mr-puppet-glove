using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProxyRotation : MonoBehaviour
{
    public Transform orient;
    public Transform root;

    [RangeAttribute(0f, 1f)]
    public float sharpness = 0.5f;

    void Update()
    {
        // Exponential ease
        root.localRotation = Quaternion.Slerp(root.localRotation, Glove.rotation, sharpness);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            orient.rotation = Quaternion.Inverse(root.localRotation);
        }
    }
}
