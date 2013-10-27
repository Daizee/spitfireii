//
// Hero.cpp
// Project Spitfire
//
// Copyright (c) 2013 Daizee (rensiadz at gmail dot com)
//
// This file is part of Spitfire.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "includes.h"
#include "Hero.h"


Hero::Hero()
{
	m_experience = m_upgradeexp = 0;
	m_id = 0;
	m_itemamount = m_itemid = 0;
	m_level = m_remainpoint = 0;
	m_stratagem = m_stratagemadded = m_stratagembuffadded = 0;
	m_power = m_poweradded = m_powerbuffadded = 0;
	m_management = m_managementadded = m_managementbuffadded = 0;
	m_status = m_loyalty = 0;
	m_name = "";
	m_logourl = "";
	movement = 0;
}

Hero::~Hero()
{

}

amf3object Hero::ToObject()
{
	amf3object obj = amf3object();
	obj["experience"] = m_experience;
	obj["id"] = m_id;
	obj["itemAmount"] = m_itemamount;
	//obj["itemId"] = m_itemid;
	obj["level"] = m_level;
	obj["logoUrl"] = m_logourl;
	obj["loyalty"] = m_loyalty;
	obj["management"] = m_management;
	obj["managementAdded"] = m_managementadded;
	obj["managementBuffAdded"] = m_managementbuffadded;
	obj["name"] = m_name;
	obj["power"] = m_power;
	obj["powerAdded"] = m_poweradded;
	obj["powerBuffAdded"] = m_powerbuffadded;
	obj["remainPoint"] = m_remainpoint;
	obj["status"] = m_status;
	obj["stratagem"] = m_stratagem;
	obj["stratagemAdded"] = m_stratagemadded;
	obj["stratagemBuffAdded"] = m_stratagembuffadded;
	obj["upgradeExp"] = m_upgradeexp;
	return obj;
}
