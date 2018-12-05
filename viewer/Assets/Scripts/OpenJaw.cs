using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenJaw : MonoBehaviour
{
    public float flexMin = 9200.0f;
    public float flexMax = 35000.0f;

    public Transform jointTop;

    [RangeAttribute(0f, 1f)]
    public float speed = 0.035f;

    public float moveRange = 10f;

    private float _flex;
    private float flexVelocity = 0f;

    void Update()
    {
        _flex = Mathf.SmoothDamp(_flex, Glove.flex, ref flexVelocity, speed);

        jointTop.localPosition = new Vector3(
            0f, Mathf.Lerp(0f, -moveRange, Mathf.InverseLerp(flexMin, flexMax, _flex)), 0f
        );

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
