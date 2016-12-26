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
	 * ��ȡ�ļ���
	 * </p>
	 * 
	 * @param filePath �ļ�·��
	 * 
	 * @return �ļ�����
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
	 * �����ļ���������ͨ����ʽ���档<br>
	 * ���filePath��·���򴴽���������ʱ����������ļ��򱣴档
	 * </p>
	 * 
	 * @param filePath �ļ�·��
	 * @param buffer ����
	 */
	public static final void saveFile(String filePath, byte[] buffer) {
		if (null == filePath || filePath.trim().equals("")) {
			return;
		}
		if (buffer == null) {
			return;
		}

		if (filePath.endsWith("/")) {
			// ������/��β������home/data/������ʾ��·������ʱӦ�����ļ���
			createFileDir(filePath);
		} else {
			createFile(filePath, buffer);
		}
	}
	
	/**
	 * <p>
	 * �����ļ�
	 * </p>
	 * 
	 * @param path �ļ�·��
	 * @param buffer �����������
	 */
	public static final void createFile(String path, byte[] buffer) {
		File file = null;
		FileOutputStream fstream = null;
		try {
			file = new File(path);
			// �жϸ�Ŀ¼�Ƿ����
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
	 * ����Ŀ¼��
	 * </p>
	 * 
	 * @param path Ŀ¼
	 */
	public static final void createFileDir(String path) {
		File file = new File(path);
		if (!file.exists()) {
			file.mkdirs();
		}
	}

	/**
	 * <p>
	 * ɾ���ļ���Ŀ¼��
	 * </p>
	 * 
	 * @param filePath �ļ�·��
	 * 
	 * @return ɾ���ɹ�����true�����򷵻�false��
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
			// ɾ��file ����
			return file.delete();
		}
		return true;
	}

	
	/**
	 * <p>
	 * �����ļ�
	 * </p>
	 * 
	 * @param in ������
	 * @param targetPath Ŀ��·�����ļ�·����
	 * 
	 * @throws IOException �׳��쳣
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
			 * BufferedOutputStreamĬ�ϻ�������С8*1024byte�������Լ�ָ����
			 */
			outBuff = new BufferedOutputStream(new FileOutputStream(targetFile, false));

			// ��������
			byte[] bytes = new byte[1024 * 5];
			int len;
			while ((len = inBuff.read(bytes)) != -1) {
				/*
				 * �鿴BufferedOutputStreamԴ���ֻ�����write��������һ������ִ��д�������
				 * ֻ��BufferedOutputStream�ڲ�������д�����ݲŻᴥ��ʵ�ʵ�д����������
				 * �ڱ���ѭ����������Ҫִ��flush�������������BufferedOutputStream�ڲ�������
				 * �в�������ûд���ļ������
				 * 
				 * PS��BufferedOutputStream.write�����Ѿ���sychronized��ʵ���������������
				 */
				outBuff.write(bytes, 0, len);
			}
			// ˢ�´˻���������
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
