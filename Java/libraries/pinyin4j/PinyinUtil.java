package com.prayerlaputa.io;

import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinCaseType;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.HanyuPinyinVCharType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;

public class PinyinUtil {
	/**
	 * ���ַ����е�����ת����ƴ���������ַ����� ����->zhangsan
	 * 
	 * @param inputString
	 * @return
	 */
	public static String getPingYin(String inputString) {
		HanyuPinyinOutputFormat format = new HanyuPinyinOutputFormat();
		// UPPERCASE����д  (ZHONG)
		// LOWERCASE��Сд  (zhong)
		format.setCaseType(HanyuPinyinCaseType.LOWERCASE); // ��ȫСд��ʽ���
		// WITHOUT_TONE��������  (zhong)
		// WITH_TONE_NUMBER��1-4���ֱ�ʾӢ��  (zhong4)
		// WITH_TONE_MARK��ֱ���������������WITH_U_UNICODE�����쳣��  (zh��ng)
		format.setToneType(HanyuPinyinToneType.WITHOUT_TONE); // ������
		// WITH_V����v��ʾ��  (nv)
		// WITH_U_AND_COLON����"u:"��ʾ��  (nu:)
		// WITH_U_UNICODE��ֱ���è� (n��)
		format.setVCharType(HanyuPinyinVCharType.WITH_V);

		char[] input = inputString.trim().toCharArray();
		String output = "";

		try {
			for (char curchar : input) {
				if (java.lang.Character.toString(curchar).matches("[\\u4E00-\\u9FA5]+")) {
					String[] temp = PinyinHelper.toHanyuPinyinStringArray(curchar, format);
					// �������Ĳ��Ǻ��֣�����null
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
	 * ����ת���ɺ���ƴ������ĸ��Ӣ���ַ����� ����->zs
	 * 
	 * @param chinese
	 *            ����
	 * @return ƴ��
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
