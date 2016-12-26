package com.prayerlaputa.io;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * 
 * @author yuchenglong8809@163.com
 * 
 * File util methos, like read file, write file, etc
 */
public class FileUtils {

	/**
	 * <p>
	 * 读取文件。
	 * </p>
	 * 
	 * @param filePath 文件路径
	 * 
	 * @return 文件数据
	 */
	public static final byte[] readFile(String filePath) {
		if(null == filePath || filePath.trim().length() <= 0){
			return null;
		}
		byte[] buffer = null;
		FileInputStream fis = null;
		try {
			File file = new File(filePath);
			if (file.exists()) {
				fis = new FileInputStream(file);
				int len = fis.available();
				buffer = new byte[len];
				fis.read(buffer);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (fis != null) {
				try {
					fis.close();
				} catch (IOException e) {
				}
			}
		}
		return buffer;
	}
	
	/**
	 * <p>
	 * 保存文件，采用普通流方式保存。<br>
	 * 如果filePath是路径则创建（不存在时）；如果是文件则保存。
	 * </p>
	 * 
	 * @param filePath 文件路径
	 * @param buffer 内容
	 */
	public static final void saveFile(String filePath, byte[] buffer) {
		if (null == filePath || filePath.trim().equals("")) {
			return;
		}
		if (buffer == null) {
			return;
		}

		if (filePath.endsWith("/")) {
			// 数据以/结尾（例：home/data/），表示是路径，此时应建立文件夹
			createFileDir(filePath);
		} else {
			createFile(filePath, buffer);
		}
	}
	
	/**
	 * <p>
	 * 创建文件
	 * </p>
	 * 
	 * @param path 文件路径
	 * @param buffer 待保存比特流
	 */
	public static final void createFile(String path, byte[] buffer) {
		File file = null;
		FileOutputStream fstream = null;
		try {
			file = new File(path);
			// 判断父目录是否存在
			if (!file.exists()) {
				File fileDir = file.getParentFile();
				fileDir.mkdirs();
			}
			file.createNewFile();
			fstream = new FileOutputStream(file);
			fstream.write(buffer);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (fstream != null) {
				try {
					fstream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	/**
	 * <p>
	 * 创建目录。
	 * </p>
	 * 
	 * @param path 目录
	 */
	public static final void createFileDir(String path) {
		File file = new File(path);
		if (!file.exists()) {
			file.mkdirs();
		}
	}

	/**
	 * <p>
	 * 删除文件或目录。
	 * </p>
	 * 
	 * @param filePath 文件路径
	 * 
	 * @return 删除成功返回true，否则返回false。
	 */
	public static final boolean deleteFile(String filePath) {
		File file = new File(filePath);
		if (!file.exists()) {
			return false;
		}
		if (file.isFile()) {
			return file.delete();
		} else if (file.isDirectory()) {
			File[] files = file.listFiles();
			if (files != null && files.length > 0) {
				for (File tempF : files) {
					deleteFile(tempF.getPath());
				}
			}
			// 删除file 本身
			return file.delete();
		}
		return true;
	}

	
	/**
	 * <p>
	 * 复制文件
	 * </p>
	 * 
	 * @param in 输入流
	 * @param targetPath 目标路径（文件路径）
	 * 
	 * @throws IOException 抛出异常
	 */
	public static final void copyFile(InputStream in, String targetPath) throws IOException {
		if (in == null || null == targetPath || targetPath.trim().equals("")) {
			return;
		}

		BufferedInputStream inBuff = null;
		BufferedOutputStream outBuff = null;

		try {
			File targetFile = new File(targetPath);
			if (!targetFile.getParentFile().exists()) {
				targetFile.getParentFile().mkdirs();
			}

			inBuff = new BufferedInputStream(in);
			/*
			 * BufferedOutputStream默认缓冲区大小8*1024byte（可以自己指定）
			 */
			outBuff = new BufferedOutputStream(new FileOutputStream(targetFile, false));

			// 缓冲数组
			byte[] bytes = new byte[1024 * 5];
			int len;
			while ((len = inBuff.read(bytes)) != -1) {
				/*
				 * 查看BufferedOutputStream源码可只，这个write方法并不一定立即执行写入操作，
				 * 只有BufferedOutputStream内部缓冲区写满数据才会触发实际的写入操作，因此
				 * 在本次循环结束后需要执行flush操作，避免出现BufferedOutputStream内部缓冲区
				 * 有部分数据没写入文件的情况
				 * 
				 * PS：BufferedOutputStream.write方法已经用sychronized（实例级别的锁）修饰
				 */
				outBuff.write(bytes, 0, len);
			}
			// 刷新此缓冲的输出流
			outBuff.flush();
		} finally {
			if (inBuff != null) {
				inBuff.close();
			}
			if (outBuff != null) {
				outBuff.close();
			}
		}
	}
	
}
