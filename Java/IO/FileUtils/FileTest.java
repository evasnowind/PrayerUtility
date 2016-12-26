package com.prayerlaputa.test;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import com.prayerlaputa.io.FileUtils;

public class FileTest {

	public static void main(String[] args) {
		
		String test = "test data";
		FileUtils.saveFile("E:\\test\\testdata.txt", test.getBytes());
		
		String test2 = new String(FileUtils.readFile("E:\\test\\testdata.txt"));
		System.out.println(test2);
		
		
		try {
			InputStream file = new FileInputStream("E:\\test\\testdata.txt");
			FileUtils.copyFile(file, "E:\\test\\testdata2.txt");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//		System.out.println(FileUtils.deleteFile("E:\\test"));
	}

}
