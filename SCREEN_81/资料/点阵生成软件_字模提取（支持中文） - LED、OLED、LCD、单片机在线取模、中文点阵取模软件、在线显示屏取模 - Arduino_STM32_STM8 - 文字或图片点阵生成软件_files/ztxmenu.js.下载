/****************************************************
 *
 *   动态上下文菜单组件, 用户可以通过自主地添加菜单项,
 *   动态地添加菜单项,设置菜单的事件处理程序等.
 *    Depend on zt.core.v.1.0.js
 *
 *   @Name ZTXMENU
 *   @Version 1.1
 *   @Date 2007-11-8
 *   @Author Lawance(Lianbao Yang, ylbslxslx@msn.com)
 *   @Copyright All Rights Reserved
 *
 ****************************************************/
var zt_oms_static_pages_cur_uri_menu = zt.base;
//if((typeof(zt_oms_static_pages_cur_uri) + "") != "undefined")zt_oms_static_pages_cur_uri_menu = zt_oms_static_pages_cur_uri;
var ZTX_MENU_DIV = '<div id="ZTXMENU_ID[MENUID]" class="ztxmenu_css_main" style="display:none;position:absolute;padding:0px 0px 0px 0px;border:1px solid #005A87;background-color:#EFEFEF;z-index:999889"></div>'+
				   '<div id="ZTXMENU_BG_ID[MENUID]" class="ztxmenu_css_bg" style="display:none;position:absolute;padding:0px 0px 0px 0px;border:0px solid #005A87;background-color:#005A87;z-index:999888">&nbsp;</div>';
var MENU_back_DIV = '<div id="BACK_DIV_FOR_MENU" class="ztxmenu_css_back" onmousedown="__HANDLE_BLACK_MM()" style="display:none;overflow:hidden;position:absolute;border:0px solid #005A87;filter:alpha(opacity=1);-moz-opacity:0.01;opacity: 0.01;left:0;top:0;width:100%;height:100%;background-color:#005A87;z-index:99988"></div>';
var AGENT = new Object();
AGENT.isML = (typeof document.implementation != 'undefined') && (typeof document.implementation.createDocument != 'undefined') && (typeof HTMLDocument!='undefined');
AGENT.isIE = window.ActiveXObject ? true : false;
AGENT.isFF = (navigator.userAgent.toLowerCase().indexOf("firefox")!=-1);
AGENT.isOP = (navigator.userAgent.toLowerCase().indexOf("opera")!=-1);
AGENT.isSTRICT = false; // 

var _ztmenus = new Array(); // 
var _menu_counter = 0;

var _cur_menu_id = 0;

function GetMenu(index)
{
	return _ztmenus[index];
}
function CreateMenu()
{
	var i = _menu_counter;
	_ztmenus[i] = new ZTXMenu();
	_ztmenus[i].index = i;
	_menu_counter++;
	return _ztmenus[i];
}

function __HANDLE_BLACK_MM()
{
	CloseMenu(_cur_menu_id);
}

/**
 *
 *  txt : 菜单项的显示文字
 *  args : 菜单项的参数，使用集中表示，格式为：{id:100,.....}
 *         id - 菜单的ID，当传入回调系统, 不传入将自动为菜单项的索引
 *         e - 菜单项是否可用 true,false
 *         v - 菜单项是否可见 true,false
 *         i - 菜单项的ICON
 *
 */
function ZTXMenuItem(txt, args)
{
	this.text = txt;
	this.index = 0;
	this.icon = (typeof(args) != "undefined") && (typeof(args.i) != "undefined") ? args.i : zt_oms_static_pages_cur_uri_menu + "ztmenu/icons/default.gif";
	this.enable = (typeof(args) != "undefined") && (typeof(args.e) != "undefined") ? args.e : true;
	this.visible = (typeof(args) != "undefined") && (typeof(args.v) != "undefined") ? args.v : true;
	this.id = (typeof(args) != "undefined") && (typeof(args.id) != "undefined") ? args.id :  -19999998;
	this.sep = (typeof(args) != "undefined") && (typeof(args.s) != "undefined") ? args.s : false;
	this.menuID = 0;
	this.subMenu = null; // 此菜单的子菜单
}
function ZTXMenu()
{
	this.items = new Array();
	this.itemCount = 0;	
	this.index = 0;
	this.bgColor = "#F5FAFC";//"#ffffff"; // 背景色
	this.borderColor = "#A5A8A8";//"#005A87"; // 边框颜色
	this.darkBgColor = "#42555C";//"#005A87"; // 阴影颜色
	this.textColor = "#444444";"#000000"; // 菜单项颜色
	this.disableColor = "#cdcdcd"; // 不可用菜单项颜色
	this.mouseMoveBgColor = "#316AC5"; //"#D6EFFC";//"#005A87"; // 鼠标移上增的背景
	this.mouseMoveColor = "#ffffff"; //"#005A87";//"#ffffff"; // 鼠标移上去的颜色
	this.darkWidth = 1; // DarkWidth
	if(AGENT.isFF || AGENT.isOP || AGENT.isML || AGENT.isSTRICT)this.darkWidth = 3; // FireFox
	this.darkHeight = 3; // DarkHeight
	this.hasDark = true; // 是否有阴影
	this.visible = false; 
	this.parentMenuItem = null; // 父亲菜单项
	this.handler = null;
}

function CloseMenu(m_id)
{
	CloseSubMenu(m_id);
	var mm = GetMenu(m_id);
	if(mm.parentMenuItem)
	{
		CloseParentMenu(mm.parentMenuItem.menuID);
	}
	mm.visible = false;
	var m = __M("ZTXMENU_ID" + m_id);
	if(m)m.style.display = "none";
	m = __M("ZTXMENU_BG_ID" + m_id);
	if(m)m.style.display = "none";
	var cc = __M("BACK_DIV_FOR_MENU");
	if(cc)cc.style.display = "none";
}

function CloseSubMenu(m_id)
{
	var mm = GetMenu(m_id);
	mm.visible = false;
	var m = __M("ZTXMENU_ID" + m_id);
	if(m)m.style.display = "none";
	m = __M("ZTXMENU_BG_ID" + m_id);
	if(m)m.style.display = "none";
		
	for(var i = 0; i < mm.itemCount; i++)
	{
		if(mm.items[i].subMenu)
		{
			CloseSubMenu(mm.items[i].subMenu.index);
		}
	}
}
function CloseParentMenu(m_id)
{
	var mm = GetMenu(m_id);
	mm.visible = false;
	var m = __M("ZTXMENU_ID" + m_id);
	if(m)m.style.display = "none";
	m = __M("ZTXMENU_BG_ID" + m_id);
	if(m)m.style.display = "none";
	
	
	var it = mm.parentMenuItem;
	if(it != null)
	{
		CloseParentMenu(it.menuID);
	}
}
function rpxm(str,o,n)
{
	var i = 0;
	i = str.indexOf(o);
	var ret = str;
	while(i>=0)
	{
		ret = ret.replace(o,n);
		i = ret.indexOf(o);
	}
	return ret;
}
function __M(id){return document.getElementById(id);}
function InitMenu(m_id)
{
	try{
		var str = rpxm(ZTX_MENU_DIV, "[MENUID]", "" + m_id);
		document.body.insertAdjacentHTML("beforeEnd", str);
	}catch(e){alert(e.message)}
}

function DisPlayDB(m_id)
{
	var cc = __M("BACK_DIV_FOR_MENU");
	var w = zt.w();
	var h = zt.h();
	if(!cc)
	{
		try{
			document.body.insertAdjacentHTML("beforeEnd", MENU_back_DIV);
			cc = __M("BACK_DIV_FOR_MENU");
			cc.style.left = 0;
			cc.style.top = 0;
			cc.style.width = w > document.body.scrollWidth ? w : document.body.scrollWidth ;
			cc.style.height = w > document.body.scrollHeight ? h : document.body.scrollHeight ;
			_cur_menu_id = m_id;
			cc.style.display = "block";
		}catch(e){}
	}else{
		if(cc.style.display == "none")
		{
			cc.style.left = 0;
			cc.style.top = 0;
			cc.style.width = w > document.body.scrollWidth ? w : document.body.scrollWidth ;
			cc.style.height = h > document.body.scrollHeight ? h : document.body.scrollHeight ;
			_cur_menu_id = m_id;
			cc.style.display = "block";
		}
	}
}

var ZTXMENUHANDLE = function(x, id, menuid){
	// Do Nothing, User can implementation this interface to handle personal MenuItem Handler
	var m = GetMenu(menuid);
	if(m)
	{
		if(m.handler)m.handler(x, id, menuid);
	}
}
function HandleMenuClick(obj, index, menuid)
{
	var m = GetMenu(menuid);
	if(typeof(m) != "undefined")
	{
		if(m.items[index].enable && m.items[index].subMenu == null)
		{
			CloseMenu(menuid);
			ZTXMENUHANDLE(obj.innerText, m.items[index].id, menuid);
		}else if(m.items[index].enable && m.items[index].subMenu != null)
		{
			m.items[index].subMenu.PopMenuAtEnd(obj);
		}
	}
}
function _HHCC(obj,index,id,menuid)
{
	try{
		window.status = __l("menustatus_prefix") + obj.innerText + __l("menustatus_sufix");
		if(window.parent)
			window.parent.status = __l("menustatus_prefix") + obj.innerText + __l("menustatus_sufix");
	}catch(e){}
	var mm = GetMenu(menuid);
	for(var i = 0; i < mm.itemCount; i++)
		if(mm.items[i].subMenu)CloseSubMenu(mm.items[i].subMenu.index);
	var it = mm.items[index];
	if(it.subMenu && it.enable)it.subMenu.PopMenuAtEnd(obj);
	
	obj.style.backgroundColor = GetMenu(menuid).mouseMoveBgColor;
	var k = __M("ZT_ITEM_TEXT_" + menuid + "" + id);	
	k.style.color = GetMenu(menuid).mouseMoveColor;
}
function _HHCCW(obj,index,id,menuid)
{
	try{
		window.status = "";
		if(window.parent)
			window.parent.status = "";
	}catch(e){}
	obj.style.backgroundColor = "";
	obj.style.color = GetMenu(menuid).textColor;//"#000000";
	var k = __M("ZT_ITEM_TEXT_" + menuid + "" + id);
	if(GetMenu(menuid).items[index].enable)
		k.style.color = GetMenu(menuid).textColor;
	else
		k.style.color = GetMenu(menuid).disableColor;
}

ZTXMenuItem.prototype.CreateSubMenu = function()
{
	this.subMenu = CreateMenu();
	GetMenu(this.menuID).CopyProperties(this.subMenu);
	this.subMenu.parentMenuItem = this; 
}

ZTXMenuItem.prototype.toString = function()
{
	if(this.sep)
	{
		return '<div class="ztxmenu_css_sep" style="border-top:1px solid #cccccc;font-size:0px;width:98%"></div>';
	}
	var str = '<div id="ZT_MENU_ITEM_'+this.id+'" class="ztxmenu_css_item" style="height:20px;width:100%;overflow:hidden;cursor:pointer;text-align:left"'+
		   ' onclick="HandleMenuClick(this, '+this.index+', '+this.menuID+')" onmouseover="_HHCC(this,'+this.index+', '+this.id+', '+this.menuID+')" onmouseout="_HHCCW(this, '+this.index+', '+this.id+', '+this.menuID+')">'+
		   '<div class="ztxmenu_css_item_img_text" style="margin-top:2px;margin-left:3px;text-align:left;float:left;overflow:hidden;">';
	str += '<img src="'+this.icon+'" width="16" height="16" border="0" align="absmiddle"/> ';
	if(this.enable)
		str += '<span class="ztxmenu_css_item_text" id="ZT_ITEM_TEXT_'+this.menuID+""+this.id+'" style="color:'+GetMenu(this.menuID).textColor+'">';
	else
		str += '<span class="ztxmenu_css_item_text_dis" id="ZT_ITEM_TEXT_'+this.menuID+""+this.id+'" style="color:'+GetMenu(this.menuID).disableColor+'">';
	str += this.text;
	str += '</span></div><div class="ztxmenu_css_item_sub" style="float:right;margin-right:4px;font-family:Webdings;color:#000000">';
	if(this.subMenu)str += "4";
	str += '</div></div>';
	return str;
}
ZTXMenuItem.prototype.SetIndex = function(i)
{
	this.index = i;
	if(this.id == -19999998)this.id = this.index;
}
___CCC = function(stxt)
{
	if(!stxt)return 0;
	var txt = EHTML(stxt);
	var j = 0;
	for(var i = 0; i < txt.length; i++)
	{
		if(txt.charCodeAt(i) < 256)j += 9;
		else j += 17;
	}
	return j + 15;
}

function EHTML(x)
{
	var str = "";
	var i = 0;
	var b = false;
	for(i = 0; i < x.length; i++)
	{
		if(x.charAt(i) == "<")
			b = true;
		else{
			if(!b)str += x.charAt(i);
			if(x.charAt(i) == ">")
			{
				b = false;
			}
		}
	}
	return str;
}

ZTXMenu.prototype.CopyProperties = function(tom)
{
	tom.bgColor = this.bgColor; // 背景色
	tom.borderColor = this.borderColor; // 边框颜色
	tom.darkBgColor = this.darkBgColor; // 阴影颜色
	tom.textColor = this.textColor; // 菜单项颜色
	tom.disableColor = this.disableColor; // 不可用菜单项颜色
	tom.mouseMoveBgColor = this.mouseMoveBgColor; // 鼠标移上增的背景
	tom.mouseMoveColor = this.mouseMoveColor; // 鼠标移上去的颜色
	tom.darkWidth = this.darkWidth; // DarkWidth
	tom.darkHeight = this.darkHeight; // DarkHeight
	tom.hasDark = this.hasDark; // 是否有阴影
}

ZTXMenu.prototype.MaxWidth = function()
{
	if(this.itemCount == 0)return 0;
	var m = ___CCC(this.items[0].text);
	for(var i = 1; i < this.itemCount; i++)
	{
		var n = ___CCC(this.items[i].text);
		if(m < n)m=n;
	}
	return m;
}

ZTXMenu.prototype.MaxHeight = function()
{
	if(this.itemCount == 0)return 0;
	var m = 0;
	for(var i = 0; i < this.itemCount; i++)
	{
		var it = this.items[i];
		if(it.sep)m += 2;
		else m += 20;
	}
	return m;
}

ZTXMenu.prototype.ItemString = function()
{
	var str = "";
	for(var i = 0; i < this.itemCount; i++)
	{
		str += "\n" + this.items[i].toString();
	}
	return str;
}

ZTXMenu.prototype.draw = function()
{
	var m = __M("ZTXMENU_ID" + this.index);
	var s = this.ItemString();
	if(typeof(this.str) == "undefined" || this.str != s)
	{
		this.str = s;
		if(m)
		{
			m.innerHTML = s;
		}
	}
}

ZTXMenu.prototype.reset = function()
{
	this.itemCount = 0;
}

ZTXMenu.prototype.enable = function(i, b)
{
	this.items[i].enable = b;
}


ZTXMenu.prototype.AddItem = function(item)
{
	item.SetIndex(this.itemCount);
	item.menuID = this.index;
	this.items[this.itemCount++] = item;
}

ZTXMenu.prototype.PM = function(x, y)
{
	var m = __M("ZTXMENU_ID" + this.index);
	var k = __M("ZTXMENU_BG_ID" + this.index);
	if(!m)
	{
		InitMenu(this.index);
		m = __M("ZTXMENU_ID" + this.index);
		k = __M("ZTXMENU_BG_ID" + this.index);
		if(!m)
		{
			alert("Can not support-MENU Version 2.1");
			return;
		}
	}
	
	
	
	m.style.width = this.MaxWidth() + "px";
	k.style.width = this.MaxWidth() + "px";
	
	var w = zt.w();
	var h = zt.h();
	var sw = zt.sl();
	var sh = zt.st();
	
	var ow = this.MaxWidth();
	var oh = this.MaxHeight(); //(m.clientHeight ? m.clientHeight : m.offsetHeight);
	
	if(x + ow > sw + w)x -= (ow + this.darkWidth + 1);
	if(y + oh > sh + h)y -= (oh + this.darkHeight + 1);
	
	m.style.left = x + "px";
	m.style.top  = y + "px";
	this.draw();
	m.style.display = "block";
	this.visible = true;
	
	k.style.height = (m.clientHeight ? m.clientHeight : m.offsetHeight) + "px";
	//alert(k.style.height + "/" + oh);
	k.style.left = (x + this.darkWidth + 1) + "px";
	k.style.top = (y + this.darkHeight + 1) + "px";
	m.style.backgroundColor = this.bgColor;
	k.style.backgroundColor = this.darkBgColor;
	m.style.borderColor = this.borderColor;
	if(this.hasDark)
		k.style.display = "block";
	else
		k.style.display = "none";
	DisPlayDB(this.index);
}
function GetROL(o)
{
	var p = o;
	var j = p.offsetLeft;
	if(p.offsetParent != null)
	{
		j += GetROL(p.offsetParent);
	}
	return j;
}
function GetROT(o)
{
	var p = o;
	var j = p.offsetTop;
	if(p.offsetParent != null)
	{
		j += GetROT(p.offsetParent);
	}
	return j;
}
// 在当前控件的左下角
ZTXMenu.prototype.PopMenuAt = function(co)
{	
	//DisPlayDB(this.index);
	var x = 0, y = 0;
	var gl = GetROL(co);
	var gt = GetROT(co);
	x = gl;
	var hy = parseInt(co.offsetHeight);
	y = hy + gt;
	this.PM(x, y);
}
// 在当前控件的左下角
ZTXMenu.prototype.PopMenuAtEnd = function(co)
{	
	//DisPlayDB(this.index);
	var x = 0, y = 0;
	var gl = zt.rl(co);
	var gt = zt.rt(co);
	x = gl + parseInt(co.offsetWidth);
	var hy = 0;//parseInt(co.offsetHeight);
	y = hy + gt;
	this.PM(x, y);
}
// 在鼠标位置弹出菜单
ZTXMenu.prototype.PopMenu = function(ee)
{
	var e = ee ? ee : (window.event ? window.event : null);
	if(!e){
		e = new Object();
		e.clientX = 10;
		e.clientY = 10;
	}
	//DisPlayDB(this.index);
	//e.clientX + __MgetScLeft(), e.clientY + __MgetScTop()
	this.PM(e.clientX + zt.sl(), e.clientY + zt.st());	
}
CreateMenu();
var ztMenu = _ztmenus[0]; // 一个MENU实例
var ztSEP  = new ZTXMenuItem("", {s:true,id:-1});

