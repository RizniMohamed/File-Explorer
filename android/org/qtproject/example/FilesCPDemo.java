
import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.support.v4.content.FileProvider;
package org.qtproject.example;

import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FilesCPDemo {
  @Override
  public void onCreate(Bundle state) {
      StrictMode.VmPolicy.Builder builder = new StrictMode.VmPolicy.Builder();
      StrictMode.setVmPolicy(builder.build());
    try{
      Method m = StrictMode.class.getMethod("disableDeathOnFileUriExposure");
      m.invoke(null);
    }catch(Exception e){
      e.printStackTrace();
    }
  }
 void static show(String name){
     System.out.println("My Name is : " + name);
}
