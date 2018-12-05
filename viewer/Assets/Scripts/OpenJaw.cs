using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenJaw : MonoBehaviour
{
    public float flexMin = 9200.0f;
    public float flexMax = 35000.0f;

    public Transform joint;
    public Transform initialPose;
    public Transform extremePose;

    [RangeAttribute(0f, 1f)]
    public float speed = 0.035f;

    public float moveRange = 10f;

    private float _flex;
    private float flexVelocity = 0f;
    private float _flex_norm;

    void Update()
    {
        _flex = Mathf.SmoothDamp(_flex, Glove.flex, ref flexVelocity, speed);
        _flex_norm = Mathf.InverseLerp(flexMin, flexMax, _flex);

        joint.localPosition = Vector3.Lerp(initialPose.localPosition, extremePose.localPosition, _flex_norm);
        joint.localRotation = Quaternion.Lerp(initialPose.localRotation, extremePose.localRotation, _flex_norm);

        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            flexMax = Glove.flex;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            flexMin = Glove.flex;
        }
    }
}
