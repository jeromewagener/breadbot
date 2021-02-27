using UnityEngine;
using UnityEngine.UI;

public class Webcam : MonoBehaviour
{
    // The raw image object gets its texture from the WebCamTexture object
    // and is used on the background panel...
    public RawImage rawImage;
    
    void Start () 
    {
        WebCamTexture webcamTexture = new WebCamTexture();
        rawImage.texture = webcamTexture;
        rawImage.material.mainTexture = webcamTexture;
        webcamTexture.Play();
    }
}