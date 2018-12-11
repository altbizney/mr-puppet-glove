using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenJaw : MonoBehaviour
{
    public int jawMin = 0;
    public int jawMax = 1023;

    public Transform joint;
    public Transform initialPose;
    public Transform extremePose;

    // [RangeAttribute(0f, 1f)]
    // public float speed = 0.035f;

    // private float _jaw;
    // private float jawVelocity = 0f;
    private float _jawNorm;

    void Update()
    {
        // _jaw = Mathf.SmoothDamp(_jaw, Glove.jaw, ref jawVelocity, speed);
        _jawNorm = Mathf.InverseLerp(jawMin, jawMax, Glove.jaw);

        joint.localPosition = Vector3.Lerp(initialPose.localPosition, extremePose.localPosition, _jawNorm);
        joint.localRotation = Quaternion.Lerp(initialPose.localRotation, extremePose.localRotation, _jawNorm);

        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            jawMin = Glove.jaw;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            jawMin = Glove.jaw;
        }
    }
}
