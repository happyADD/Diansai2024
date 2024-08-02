// JavaScript Document
var DEPBrowser = new Object();
DEPBrowser.isMozilla = (typeof document.implementation != 'undefined') && (typeof document.implementation.createDocument != 'undefined') && (typeof HTMLDocument!='undefined');
DEPBrowser.isIE = window.ActiveXObject ? true : false;
DEPBrowser.isFirefox = (navigator.userAgent.toLowerCase().indexOf("firefox")!=-1);
DEPBrowser.isOpera = (navigator.userAgent.toLowerCase().indexOf("opera")!=-1);
function DEPMenuItem(_name, _url, _target)
{
	this.name = _name;
	this.url = _url;
	this.target = _target == "" ? "_self" : _target;
}
var _memMenuID = 0;
// key: key, it:item
function DEPMenuMem(key)
{
	this.items = [];
	this.length = 0;
	this.index = _memMenuID++;
	this.addItem = function(v)
	{
		this.items[this.length] = v;
		this.length++;
	}
	this.getMenuString = function()
	{
		var str = "<ul class=\"zt_sys_sub_menu\">";
		for(var i = 0; i < this.length; i++)
		{
			str += '<li class="zt_sys_sub_menu_item">';
			str += '<a href="'+this.items[i].url+'" target="'+this.items[i].target+'">';
			str += this.items[i].name;
			str += "</a></li>";
		}
		str += "</ul>";
		return str;
	}
	this.maxWidth = function()
	{
		var str = "";
		var mm = 0;
		for(var i = 0; i < this.length; i++)
		{
			str = this.items[i].name;
			if(_menu_str_width(str) > mm)mm = _menu_str_width(str);
		}
		return mm;
	}
	this.totalWidth = function()
	{
		var str = "";
		var mm = 0;
		for(var i = 0; i < this.length; i++)
		{
			str = this.items[i].name;
			mm += _menu_str_width(str);
		}
		return mm;
	}
}
function _menu_str_width(s)
{	
	var l = 0;
	for(var i = 0; i < s.length; i++)
	{
		if(s.charCodeAt(i) < 255 && s.charCodeAt(i) > 0)l += 9;
		else l += 13;
	}
	return l;
}

var _depmenus = [];

function DEPAddMenuItem(subId, _name, _url, _target)
{
	if(!_depmenus[subId])
	{
		_depmenus[subId] = new DEPMenuMem(subId);
	}
	_depmenus[subId].addItem(new DEPMenuItem(_name, _url, _target));
}
function _gm(id){return document.getElementById(id);}
//
function DEPMenu()
{
	this.obj = null;
	this.lastActive = 0;
	this.offtop = 23;
	this.offleft = 0;
	this.showType = 0; // 0 - DropMenu, 1 - Left to right menu
	this.minWidth = 80;
	this.appendWidth = 20;
	this.minTWidth = 500; // 当前菜单在父亲窗口的左边坐标最小值，大于此值后菜单向左显示
	this.curobj = null;
	var f = this;
	this.createMenu = function()
	{		
		if(DEPBrowser.isIE){
			if(document.readyState != "complete")return;
		}
		if(this.obj)return;
		this.obj = document.createElement("DIV");
		this.obj.id = "zt_oms_menu_obj";
		this.obj.style.cssText = "position:absolute;display:none";
		document.body.appendChild(this.obj);
		if(this.obj)
		{
			this.obj.onmousemove = function(){ // 鼠标移动时改变最后活动时间
				f.lastActive = new Date().getTime();
			}
		}else{alert("Not Support DEP Menu");}
	}
	this.PopAT = function(o, subId)
	{
		this.hide();
		this.curobj = o;
		try{if(TIMER_DEP_MENU)clearTimeout(TIMER_DEP_MENU);}catch(e){} // 清除关闭时钟
		this.createMenu();
		if(!this.obj)return;
		this.lastActive = new Date().getTime();
		this.obj.style.left = (parseInt(DEPGetROL(o)) + this.offleft) + "px";
		this.obj.style.top  = (parseInt(DEPGetROT(o)) + this.offtop) + "px";
		//alert(o.parentNode);
		var poleft = parseInt(DEPGetROL(o.parentNode));
		if(_depmenus[subId])
		{
			var wd = _depmenus[subId].maxWidth();
			var tw = _depmenus[subId].totalWidth();			
			if(this.showType == 1){
				wd = tw;
				wd += this.appendWidth * _depmenus[subId].length;
				//alert(_depmenus[subId].index);
				if((parseInt(DEPGetROL(o)) - poleft) > this.minTWidth)
				{
					this.obj.style.left = (parseInt(DEPGetROL(o)) - wd + o.offsetWidth + this.offleft) + "px";
				}
			}
			if(wd < this.minWidth)wd = this.minWidth;			
			this.obj.style.width = wd + "px";
			this.obj.innerHTML = _depmenus[subId].getMenuString();
			this.obj.style.display = "block";
			o["class"] = "act";
			o["className"] = "act";
		}else{
			this.obj.style.display = "none";
			o["class"] = "";
			o["className"] = "";
		}
	}
	this.needClose = function()
	{
		if(new Date().getTime() - this.lastActive >= 1000) // 小于五秒就关闭
		{
			return true;
		}
		return false;
	}
	this.hide = function()
	{
		if(this.obj){
			this.obj.style.display = "none";	
			if(this.curobj)
			{
				this.curobj["class"] = "";
				this.curobj["className"] = "";
			}
		}
	}
	this.SetOffsetTop = function(t)
	{
		this.offtop = t;
	}
}

var _depMenu = new DEPMenu();
//_depMenu.createMenu();

function showDepMenu(id, obj)
{
	_depMenu.PopAT(obj, id);
}
var TIMER_DEP_MENU = null;
function hideDepMenu()
{
	if(_depMenu.needClose())
	{
		_depMenu.hide();
		return;
	}
	try{if(TIMER_DEP_MENU)clearTimeout(TIMER_DEP_MENU);}catch(e){}
	TIMER_DEP_MENU = setTimeout("hideDepMenu()", 100);
}

function DEPGetROL(o)
{
	var p = o;
	var j = p.offsetLeft;
	if(p.offsetParent != null)
	{
		j += DEPGetROL(p.offsetParent);
	}
	return j;
}
function DEPGetROT(o)
{
	var p = o;
	var j = p.offsetTop;
	if(p.offsetParent != null)
	{
		j += DEPGetROT(p.offsetParent);
	}
	return j;
}




