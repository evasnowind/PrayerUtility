package com.prayer.testdemo;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.text.TextUtils;
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
	
	private static void saveImageByUri(Activity activity, Uri uri, String destPath, String name){
		InputStream is = null;
		
		try {
			is = activity.getContentResolver().openInputStream(uri);
			
			saveImageToSD(is, destPath, name);
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} finally{
			if(is != null){
				try {
					is.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	

	private static void saveImageToSD(InputStream is, String destPath, String imageName){
		String sdPath = Environment.getExternalStorageDirectory().getPath();
		
		//TODO ++++++++++++++++++++++++++++++++++++++++++
		//添加SD相关判断
		Bitmap bitmap = BitmapFactory.decodeStream(is);
		
		File file = new File(sdPath+"/"+destPath);
		File picFile = null;
	
		FileOutputStream fos = null;
		try {
			
			picFile = new File(file, imageName);
			if(!picFile.exists()){
				picFile.createNewFile();
			}
			
			fos = new FileOutputStream(picFile);
			
			if(!TextUtils.isEmpty(imageName) && imageName.endsWith(".png")){
				bitmap.compress(CompressFormat.PNG, 100, fos);
			} else if(!TextUtils.isEmpty(imageName) 
					&& (imageName.endsWith(".jpg") || imageName.endsWith(".jpeg"))){
				bitmap.compress(CompressFormat.JPEG, 100, fos);
			} else {
				//log, throw exception or do something
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally{
			if(fos != null){
				try {
					fos.flush();
					fos.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	/*
	 * 以下代码取自：Android 将文件保存到SD卡，从卡中取文件，及删除文件
	 * 地址：http://www.cnblogs.com/dyllove98/archive/2013/07/01/3165584.html 
	 */
	
	//保存到SD卡  
	private static String sdState = Environment.getExternalStorageState();
	private static String path = Environment.getExternalStorageDirectory().toString();

	public static void saveBitmap(Bitmap bitmap, String imageName) {
		File file;
		File PicName;
		if (sdState.equals(Environment.MEDIA_MOUNTED)) {
			// 获得sd卡根目录
			file = new File(path + "/Huai/TicketsPic");
			if (!file.exists()) {
				file.mkdirs();
			}
			PicName = new File(file, imageName);
			try {
				if (!PicName.exists()) {
					PicName.createNewFile();
				}
				FileOutputStream fos = new FileOutputStream(PicName);
				if (PicName.getName().endsWith(".png")) {
					bitmap.compress(CompressFormat.PNG, 100, fos);
				} else if (PicName.getName().endsWith(".jpg")) {
					bitmap.compress(CompressFormat.JPEG, 100, fos);
				}
				fos.flush();
				fos.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	// 从SD卡取
	public static Bitmap getBitmap(String imageName) {
		Bitmap bitmap = null;
		File imagePic;
		if (sdState.equals(Environment.MEDIA_MOUNTED)) {

			imagePic = new File(path + "/Huai/TicketsPic", imageName);
			if (imagePic.exists()) {
				try {
					bitmap = BitmapFactory.decodeStream(new FileInputStream(imagePic));
				} catch (FileNotFoundException e) {
					// e.printStackTrace();
				}
			}
		}
		return bitmap;
	}

	// 将SD卡文件删除
	public static void deleteFile(File file) {
		if (sdState.equals(Environment.MEDIA_MOUNTED)) {
			if (file.exists()) {
				if (file.isFile()) {
					file.delete();
				}
				// 如果它是一个目录
				else if (file.isDirectory()) {
					// 声明目录下所有的文件 files[];
					File files[] = file.listFiles();
					for (int i = 0; i < files.length; i++) { // 遍历目录下所有的文件
						deleteFile(files[i]); // 把每个文件 用这个方法进行迭代
					}
				}
				file.delete();
			}
		}
	}

}
