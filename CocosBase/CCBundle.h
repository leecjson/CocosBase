/****************************************************************************
Copyright (c) 2014 viva-Lijunlin

Created by Li JunLin on 2014

csdn_viva@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCBASE_BUNDLE_H__
#define __CCBASE_BUNDLE_H__

/////////////////////////////////////////////////////////////////////////////
/// BugFix : 
///
/////////////////////////////////////////////////////////////////////////////

#include "cocos2d.h"
#include <map>
#include <string>

NS_CC_BEGIN

/**
 * class    : CCBundle
 * author   : viva - Lijunlin
 * email    : csdn_viva@foxmail.com
 * function : 多类型键值对数据结构
 */
class CCBundle : public CCObject
{
public:
	CCBundle();
	virtual ~CCBundle();
	static CCBundle* create();

public:
	// 键值内容结构定义
	struct tagValue
	{
		// 值类型
		int nType;
		// 值的指针变量
		void* pValue;
	};

public:
	void putShort(const char* key, short value);
	void putUShort(const char* key, unsigned short value);
	void putInt(const char* key, int value);
	void putUInt(const char* key, unsigned int value);
	void putFloat(const char* key, float value);
	void putDouble(const char* key, double value);
	void putString(const char* key, const char* value);
	void putBundle(const char* key, CCBundle* value);
	void putObject(const char* key, CCObject* value);
	void putParam(const char* key, void* value);

public:
	short getShort(const char* key);
	unsigned short getUShort(const char* key);
	int getInt(const char* key);
	unsigned int getUInt(const char* key);
	float getFloat(const char* key);
	double getDouble(const char* key);
	const char* getString(const char* key);
	CCBundle* getBundle(const char* key);
	CCObject* getObject(const char* key);
	void* getParam(const char* key);

protected:
	virtual CCObject* copyWithZone(CCZone* pZone);

private:
	void removeValueByKey(const char* key);
	void removeValue(const tagValue& tValue);

	std::map<std::string, tagValue> m_mDatas;
};

NS_CC_END

#endif //__CCBASE_BUNDLE_H__