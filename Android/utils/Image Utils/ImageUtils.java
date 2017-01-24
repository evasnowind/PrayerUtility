package com.prayer.testdemo;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;

public class ImageUtils {

	public static final int ALBUM_IMAGE_REQ_CODE = 1000;
	
	public static void getImageFromAlbum(Activity activty, int requestCode) {
		Intent intent = new Intent(Intent.ACTION_PICK);
		intent.setType("image/*");// 相片类型
		activty.startActivityForResult(intent, requestCode);
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data){
		if(Activity.RESULT_OK == resultCode){
			if( ALBUM_IMAGE_REQ_CODE == requestCode){
				//do something to the album image
				Uri imageUri = data.getData();
			}
		}
		
	}

	//storageUri必须是一个SD卡地址，以便保存清晰照片
	protected void getImageFromCamera(Activity activity, int requestCode, Uri storageUri) {
		String state = Environment.getExternalStorageState();
		if (state.equals(Environment.MEDIA_MOUNTED)) {
			Intent getImageByCamera = new Intent("android.media.action.IMAGE_CAPTURE");
			getImageByCamera.putExtra(MediaStore.EXTRA_OUTPUT, storageUri);
			activity.startActivityForResult(getImageByCamera, requestCode);
		} else {
			Toast.makeText(activity.getApplicationContext(), "请确认已经插入SD卡", Toast.LENGTH_LONG).show();
		}
	}

	//辅助方法，用于生成一个SD卡路径的Uri对象
	private static Uri getImageStorageUri(String sdSaveDir, String imageName){
		Uri uri = null;
		String sdState = Environment.getExternalStorageState();
		if(sdState.equals(Environment.MEDIA_MOUNTED)){
			String imagePath = Environment.getExternalStorageDirectory().getPath()+"/"+sdSaveDir;
			File file = new File(imagePath);
			if(!file.exists()){
				file.mkdir();
			}
			imagePath = imagePath + "/"+imageName;
			uri = Uri.fromFile(new File(imagePath));
		} else {
			//TODO throw exception or do something
		}
		return uri;
	}
}
