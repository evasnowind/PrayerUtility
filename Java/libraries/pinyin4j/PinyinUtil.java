package com.prayerlaputa.io;

import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinCaseType;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.HanyuPinyinVCharType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;

public class PinyinUtil {
	/**
	 * 将字符串中的中文转换成拼音，其他字符不变 张三->zhangsan
	 * 
	 * @param inputString
	 * @return
	 */
	public static String getPingYin(String inputString) {
		HanyuPinyinOutputFormat format = new HanyuPinyinOutputFormat();
		// UPPERCASE：大写  (ZHONG)
		// LOWERCASE：小写  (zhong)
		format.setCaseType(HanyuPinyinCaseType.LOWERCASE); // 以全小写方式输出
		// WITHOUT_TONE：无音标  (zhong)
		// WITH_TONE_NUMBER：1-4数字表示英标  (zhong4)
		// WITH_TONE_MARK：直接用音标符（必须WITH_U_UNICODE否则异常）  (zhòng)
		format.setToneType(HanyuPinyinToneType.WITHOUT_TONE); // 无声调
		// WITH_V：用v表示ü  (nv)
		// WITH_U_AND_COLON：用"u:"表示ü  (nu:)
		// WITH_U_UNICODE：直接用ü (nü)
		format.setVCharType(HanyuPinyinVCharType.WITH_V);

		char[] input = inputString.trim().toCharArray();
		String output = "";

		try {
			for (char curchar : input) {
				if (java.lang.Character.toString(curchar).matches("[\\u4E00-\\u9FA5]+")) {
					String[] temp = PinyinHelper.toHanyuPinyinStringArray(curchar, format);
					// 如果输入的不是汉字，返回null
					if (null != temp && temp.length >= 1) {
						output += temp[0];
					}
				} else {
					output += java.lang.Character.toString(curchar);
				}
			}
		} catch (BadHanyuPinyinOutputFormatCombination e) {
			e.printStackTrace();
		}
		return output;
	}

	/**
	 * 汉字转换成汉语拼音首字母，英文字符不变 张三->zs
	 * 
	 * @param chinese
	 *            汉字
	 * @return 拼音
	 */
	public static String getFirstSpell(String chinese) {
		StringBuffer pybf = new StringBuffer();
		char[] arr = chinese.toCharArray();
		HanyuPinyinOutputFormat defaultFormat = new HanyuPinyinOutputFormat();
		defaultFormat.setCaseType(HanyuPinyinCaseType.LOWERCASE);
		defaultFormat.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
		for (char curchar : arr) {
			if (curchar > 128) {
				try {
					String[] temp = PinyinHelper.toHanyuPinyinStringArray(curchar, defaultFormat);
					if (temp != null) {
						pybf.append(temp[0].charAt(0));
					}
				} catch (BadHanyuPinyinOutputFormatCombination e) {
					e.printStackTrace();
				}
			} else {
				pybf.append(curchar);
			}
		}
		return pybf.toString().replaceAll("\\W", "").trim();
	}
}
