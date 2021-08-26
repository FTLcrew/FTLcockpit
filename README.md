# **FTL Spacecrew Universal Cockpit Kontroller System**

A custom plugin written in C# for a KSP controller. Written for the FTL Cockpit. 

## **Instructions**

To be able to compile this project in Visual Studio, open the project solution and right click on Dependencies in the Solution Explorer.
* Click on "Add Project Reference" to open the Reference Manager.
* Click Browse...
* Browse to “<KSP_Install>\KSP_x64_Data\Managed” and add a reference to UnityEngine.dll, UnityEngine.UI.dll, UnityEngine.CoreModule.dll, Assembly-Csharp.dll and Assembly-Csharp-firstpass.dll.
	* **UnityEngine.CoreModule.dll is required since KSP 1.8+!!!**