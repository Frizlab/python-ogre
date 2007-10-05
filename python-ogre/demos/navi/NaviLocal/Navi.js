function PageNaviData()
{
	var url = window.document.URL.toString();
	
	this.getName = getName;
	this.doesExist = doesExist;
	this.get = get;
	
	function getName()
	{
		if(url.indexOf("?") > -1 && url.indexOf("?", url.indexOf("?")+1) > -1)
			return url.substr(url.indexOf("?")+1).split("?")[0];

		return "";
	}
	
	function doesExist(ParameterName)
	{
		if(get(ParameterName).length)
			return true;
		else
			return false;
	}
		
	function get(ParameterName)
	{
		if(url.indexOf("?") > -1 && url.indexOf("?", url.indexOf("?")+1) > -1)
		{
			var Parameters = url.substr(url.indexOf("?", url.indexOf("?")+1)+1).split("&");
			for (i = 0; i < Parameters.length; i++)
			{
				if(Parameters[i].indexOf("=") > 0)
				{
					var ParameterValue = Parameters[i].split("=");
					if(ParameterValue[0] == ParameterName) 
						return decodeURIComponent(ParameterValue[1]);
				}
			}
		}
		return "";
	}
}

pageNaviData = new PageNaviData();

function NaviData(naviDataName)
{
	var dataString = "NAVI_DATA:?" + naviDataName + "?";
	var paramCount = 0;
	this.add = add;
	this.addQS = addQS;
	this.send = send;
	
	function add(paramName, paramValue)
	{
		if(paramCount != 0)
			dataString += "&";
		
		if(isNaN(paramValue))
			dataString += paramName + "=" + encodeURIComponent(paramValue);
		else
			dataString += paramName + "=" + paramValue;
			
		paramCount++;
		
		return this;
	}
	
	function addQS(queryString)
	{
		if(paramCount != 0)
			dataString += "&";
			
		if(queryString != "")
			dataString += queryString;
			
		paramCount++;
		
		return this;
	}
	
	function send()
	{
		window.statusbar = false;
		window.status = dataString;
		
		if(window.console)
		{
			console.group("NaviData sent!");
			console.log("Name: " + naviDataName);
			console.log("Data: " + dataString);
			console.groupEnd();
		}
		
		return this;
	}
}
	
function getTime()
{
	var suffix = "";
	var now = new Date();
	var hours = now.getHours();
	
	if (hours < 12)
		suffix = "AM";
	else
		suffix = "PM";
	
	if (hours == 0)
		hours = 12;
		
	if (hours > 12)
		hours = hours - 12;

	var minutes = String(now.getMinutes());

	if (minutes.length == 1)
		minutes = "0" + minutes;

	return String(String(hours) + ":" + minutes + " " + suffix);
}

function getNaviName()
{
	return navigator.userAgent.substr(navigator.userAgent.lastIndexOf(" "));
}