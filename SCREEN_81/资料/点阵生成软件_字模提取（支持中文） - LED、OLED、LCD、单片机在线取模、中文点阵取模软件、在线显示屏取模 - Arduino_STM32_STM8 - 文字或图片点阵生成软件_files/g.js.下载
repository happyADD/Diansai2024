/***
 * ZTGraph
 */
var zt_g_lists = {};
function ZTGraph(id)
{
	this.id = id;
	this.loaded = true;
	this.ctl = get_zt_graph_object(id);
	
	this.Load = function(url)
	{
		try{
			this.ctl.Load(url);
		}catch(Xc){}
	}
	this.AddNode = function(id,n,v)
	{
		try{
			var gid = arguments.length > 3 ? arguments[3] : "";
			var sid = arguments.length > 4 ? arguments[4] : "";
			var unit = arguments.length > 5 ? arguments[5] : "";
			this.ctl.AddNode(id,n,v,gid,sid,unit);
		}catch(Xc){}
	}
	this.AddGroup = function(id,n)
	{
		try{
			this.ctl.AddGroup(id,n);
		}catch(Xc){}
	}
	this.AddSerial = function(id,n)
	{
		try{
			this.ctl.AddSerial(id,n);
		}catch(Xc){}
	}
	this.AddColor = function(c)
	{
		try{
			this.ctl.AddColor(c);
		}catch(Xc){}
	}
	this.SetColor = function(tp)
	{
		try{
			var st = arguments.length > 1 ? arguments[1] : 0xff0000;
			var et = arguments.length > 2 ? arguments[2] : 0;
			this.ctl.SetCS(tp, st, et);
		}catch(Xc){}
	}
	this.Redraw = function()
	{
		try{
			this.ctl.build();
		}catch(Xc){}
	}
	this.Clear = function()
	{
		try{
			this.ctl.clear();
		}catch(Xc){}
	}
	this.AddColors = function(cs)
	{
		try{
			this.ctl.AddColors(cs);
		}catch(Xc){}
	}
	this.AddColors = function(cs)
	{
		try{
			this.ctl.AddColors(cs);
		}catch(Xc){}
	}
	this.DrawType = function(t)
	{
		try{
			this.ctl.DrawType(t);
		}catch(Xc){}
	}
	this.Config = function()
	{
		try{
			if(arguments.length > 0 && arguments.length % 2 == 0)
			{
				for(var i = 0; i < arguments.length; i+=2)
				{
					try{
						this.ctl.config(arguments[i], arguments[i+1]);
					}catch(Xc){}
				}
			}
		}catch(Xc){}
	}
	this.Node = function()
	{
		try{
			if(arguments.length > 2 && arguments.length % 2 == 1)
			{
				for(var i = 1; i < arguments.length; i+=2)
				{
					try{
						this.ctl.node(arguments[0], arguments[i], arguments[i+1]);
					}catch(Xc){}
				}
			}
		}catch(Xc){}
	}
}
function get_zt_graph_object(id) {
	if (navigator.appName.indexOf("Microsoft") != -1) {
		return zt.g(id);
	}
	else {
		return document[id];
	}
}
function ZTGraphLoaded(drawerid,ctrlid)
{
	var z = new ZTGraph(drawerid);
	zt_g_lists[z.id] = z;
	if(window["ZTGraphLoadCallback"])
	{
		window["ZTGraphLoadCallback"](z);
	}
	if(window["ZTGraphLoadCallback_" + drawerid])
	{
		window["ZTGraphLoadCallback_" + drawerid](z);
	}
}
function ZTGraphXMLLoaded(drawerid,xml)
{
	var g = ztg(drawerid);
	if(g){
		if(window["ZTGraphXMLLoadCallback_"+drawerid])
		{
			window["ZTGraphXMLLoadCallback_"+drawerid](g,xml);
		}
	}
}
function AddG(id,w,h)
{
	var dt = "";
	if(arguments.length == 4){
		dt = arguments[3];
		dt = escape(dt);
	}	
	var rd = Math.random();
	var s = '<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" width="'+w+'" height="'+h+'" id="'+id+'" align="middle">';
		s += '<param name="allowScriptAccess" value="sameDomain" />';
		s += '					<param name="allowFullScreen" value="true" />';
		s += '					<param name="wmode" value="opaque">';
		s += '					<param name="movie" value="verip_private/g/ZTGraph.swf?rd='+rd+'&drawerid='+id+'&datas='+dt+'" /><param name="quality" value="high" /><param name="bgcolor" value="#ffffff" />	';
		s += '					<embed src="verip_private/g/ZTGraph.swf?rd='+rd+'&drawerid='+id+'&datas='+dt+'" quality="high" bgcolor="#ffffff" ';
		s += ' width="'+w+'" height="'+h+'" wmode="opaque" name="'+id+'" align="middle" allowScriptAccess="sameDomain" allowFullScreen="true" type="application/x-shockwave-flash"/>';
		s += '					</object>';
	document.write(s);
}
function ztg(id)
{
	return zt_g_lists[id];
}