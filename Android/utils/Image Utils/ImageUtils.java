package com.prayer.testdemo;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
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
	
	
	
	/*
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 * 
	 * TODO
	 * 下面有关缩放图片的方法有待验证
	 * TODO
	 * 
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */
	 /** 
	  * 缩放Bitmap满屏 
	  * 
	  * @param bitmap 
	  * @param screenWidth 
	  * @param screenHight 
	  * @return 
	  */  
	public static Bitmap getBitmap(Bitmap bitmap, int screenWidth, int screenHight) {
		int w = bitmap.getWidth();
		int h = bitmap.getHeight();
		Matrix matrix = new Matrix();
		float scale = (float) screenWidth / w;
		float scale2 = (float) screenHight / h;
		// scale = scale < scale2 ? scale : scale2;
		matrix.postScale(scale, scale);
		Bitmap bmp = Bitmap.createBitmap(bitmap, 0, 0, w, h, matrix, true);
		if (bitmap != null && !bitmap.equals(bmp) && !bitmap.isRecycled()) {
			bitmap.recycle();
			bitmap = null;
		}
		return bmp;// Bitmap.createBitmap(bitmap, 0, 0, w, h, matrix, true);
	}

	private static String TAG = "test";

	/**
	 * 按最大边按一定大小缩放图片
	 */
	public static Bitmap scaleImage(byte[] buffer, float size) {
		// 获取原图宽度
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		options.inPurgeable = true;
		options.inInputShareable = true;
		Bitmap bm = BitmapFactory.decodeByteArray(buffer, 0, buffer.length, options);
		// 计算缩放比例
		float reSize = options.outWidth / size;
		if (options.outWidth < options.outHeight) {
			reSize = options.outHeight / size;
		}
		// 如果是小图则放大
		if (reSize <= 1) {
			int newWidth = 0;
			int newHeight = 0;
			if (options.outWidth > options.outHeight) {
				newWidth = (int) size;
				newHeight = options.outHeight * (int) size / options.outWidth;
			} else {
				newHeight = (int) size;
				newWidth = options.outWidth * (int) size / options.outHeight;
			}
			bm = BitmapFactory.decodeByteArray(buffer, 0, buffer.length);
			bm = scaleImage(bm, newWidth, newHeight);
			if (bm == null) {
				Log.e(TAG, "convertToThumb, decode fail:" + null);
				return null;
			}
			return bm;
		}
		// 缩放
		options.inJustDecodeBounds = false;
		options.inSampleSize = (int) reSize;
		bm = BitmapFactory.decodeByteArray(buffer, 0, buffer.length, options);
		if (bm == null) {
			Log.e(TAG, "convertToThumb, decode fail:" + null);
			return null;
		}
		return bm;
	}

	/**
	 * 按新的宽高缩放图片
	 * 
	 * @param bm
	 * @param newWidth
	 * @param newHeight
	 * @return
	 */
	public static Bitmap scaleImage(Bitmap bm, int newWidth, int newHeight) {
		if (bm == null) {
			return null;
		}
		int width = bm.getWidth();
		int height = bm.getHeight();
		float scaleWidth = ((float) newWidth) / width;
		float scaleHeight = ((float) newHeight) / height;
		Matrix matrix = new Matrix();
		matrix.postScale(scaleWidth, scaleHeight);
		Bitmap newbm = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, true);
		if (bm != null & !bm.isRecycled()) {
			bm.recycle();
			bm = null;
		}
		return newbm;
	}
	
	
	//使用BitmapFactory.Options的inSampleSize参数来缩放
    public static Drawable resizeImage(String path,
            int width,int height) 
    {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;//不加载bitmap到内存中
        BitmapFactory.decodeFile(path,options); 
        int outWidth = options.outWidth;
        int outHeight = options.outHeight;
        options.inDither = false;
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        options.inSampleSize = 1;
         
        if (outWidth != 0 && outHeight != 0 && width != 0 && height != 0) 
        {
            int sampleSize=(outWidth/width+outHeight/height)/2;
            Log.d("mydebug", "sampleSize = " + sampleSize);
            options.inSampleSize = sampleSize;
        }
     
        options.inJustDecodeBounds = false;
        return new BitmapDrawable(BitmapFactory.decodeFile(path, options));     
    }

	/**
	 * 以屏幕宽度为基准，显示图片
	 * 
	 * @param iv
	 * @param path
	 * @param screenW
	 * @return
	 */
	public static Bitmap decodeStream(Context context, Intent data, float size) {
		Bitmap image = null;
		try {
			Uri dataUri = data.getData();
			// 获取原图宽度
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inJustDecodeBounds = true;
			options.inPurgeable = true;
			options.inInputShareable = true;
			BitmapFactory.decodeStream(context.getContentResolver().openInputStream(dataUri), null, options);
			// 计算缩放比例
			float reSize = (int) (options.outWidth / size);
			if (reSize <= 0) {
				reSize = 1;
			}
			Log.d(TAG, "old-w:" + options.outWidth + ", llyt-w:" + size + ", resize:" + reSize);
			// 缩放
			options.inJustDecodeBounds = false;
			options.inSampleSize = (int) reSize;
			image = BitmapFactory.decodeStream(context.getContentResolver().openInputStream(dataUri), null, options);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return image;
	}

	/**
	 * 检查图片是否超过一定值，是则缩小
	 * 
	 * @param view
	 * @param strFileName
	 */
	public static Bitmap convertToThumb(byte[] buffer, float size) {
		// 获取原图宽度
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		options.inPurgeable = true;
		options.inInputShareable = true;
		Bitmap bm = BitmapFactory.decodeByteArray(buffer, 0, buffer.length, options);
		// 计算缩放比例
		float reSize = options.outWidth / size;
		if (options.outWidth > options.outHeight) {
			reSize = options.outHeight / size;
		}
		if (reSize <= 0) {
			reSize = 1;
		}
		Log.d(TAG, "convertToThumb, reSize:" + reSize);
		// 缩放
		options.inJustDecodeBounds = false;
		options.inSampleSize = (int) reSize;
		if (bm != null && !bm.isRecycled()) {
			bm.recycle();
			bm = null;
			Log.e(TAG, "convertToThumb, recyle");
		}
		bm = BitmapFactory.decodeByteArray(buffer, 0, buffer.length, options);
		if (bm == null) {
			Log.e(TAG, "convertToThumb, decode fail:" + null);
			return null;
		}
		return bm;
	}

	/**
	 * 图片的质量压缩方法
	 * 
	 * @param image
	 * @return
	 */
	public static Bitmap compressImage(Bitmap image) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		image.compress(Bitmap.CompressFormat.JPEG, 100, baos);// 质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
		int options = 100;
		while (baos.toByteArray().length / 1024 > 100) { // 循环判断如果压缩后图片是否大于100kb,大于继续压缩
			baos.reset();// 重置baos即清空baos
			image.compress(Bitmap.CompressFormat.JPEG, options, baos);// 这里压缩options%，把压缩后的数据存放到baos中
			options -= 10;// 每次都减少10
		}
		ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());// 把压缩后的数据baos存放到ByteArrayInputStream中
		Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);// 把ByteArrayInputStream数据生成图片
		if (baos != null) {
			try {
				baos.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (isBm != null) {
			try {
				isBm.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (image != null && !image.isRecycled()) {
			image.recycle();
			image = null;
		}
		return bitmap;
	}

	/**
	 * 图片按比例大小压缩方法(根据Bitmap图片压缩)
	 * 
	 * @param image
	 * @return
	 */
	public static Bitmap getImage(Bitmap image) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
		if (baos.toByteArray().length / 1024 > 1024) {// 判断如果图片大于1M,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出
			baos.reset();// 重置baos即清空baos
			image.compress(Bitmap.CompressFormat.JPEG, 50, baos);// 这里压缩50%，把压缩后的数据存放到baos中
		}
		ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());
		BitmapFactory.Options newOpts = new BitmapFactory.Options();
		// 开始读入图片，此时把options.inJustDecodeBounds 设回true了
		newOpts.inJustDecodeBounds = true;
		Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
		newOpts.inJustDecodeBounds = false;
		int w = newOpts.outWidth;
		int h = newOpts.outHeight;
		// 现在主流手机比较多是800*480分辨率，所以高和宽我们设置为
		float hh = 800f;// 这里设置高度为800f
		float ww = 480f;// 这里设置宽度为480f
		// 缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
		int be = 1;// be=1表示不缩放
		if (w > h && w > ww) {// 如果宽度大的话根据宽度固定大小缩放
			be = (int) (newOpts.outWidth / ww);
		} else if (w < h && h > hh) {// 如果高度高的话根据宽度固定大小缩放
			be = (int) (newOpts.outHeight / hh);
		}
		if (be <= 0)
			be = 1;
		newOpts.inSampleSize = be;// 设置缩放比例
		// 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
		isBm = new ByteArrayInputStream(baos.toByteArray());
		bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
		if (isBm != null) {
			try {
				isBm.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (image != null && !image.isRecycled()) {
			image.recycle();
			image = null;
		}
		return compressImage(bitmap);// 压缩好比例大小后再进行质量压缩
	}
}
