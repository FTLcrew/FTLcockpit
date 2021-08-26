using System;
using UnityEngine;

namespace HelloWorld
{
    [KSPAddon(KSPAddon.Startup.MainMenu, false)]
    public class Hello : MonoBehaviour
    {
        public void Update()
        {
            Debug.Log("Hello world! " + Time.realtimeSinceStartup);
        }
    }
}