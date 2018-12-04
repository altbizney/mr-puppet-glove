using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProxyRotation : MonoBehaviour
{
    public Transform orient;
    public Transform root;

    void Update()
    {
        root.localRotation = wrmhlRead.rotation;

        if (Input.GetKeyDown(KeyCode.Space))
        {
            orient.rotation = Quaternion.Inverse(root.localRotation);
        }
    }
}
