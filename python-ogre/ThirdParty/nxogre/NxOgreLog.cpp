//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreLog.h"					
#include "NxOgreError.h"				// For: Log inherits ErrorReporter
#include "NxOgrePhysXDriver.h"			// For: World Access
#include "NxOgreWorld.h"				// For: World->Shutodwn

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

Log::Log(logType t) : nbReports(0) {
	
	mType = t;

	if (mType == TEXT) {
		openAsText("NxOgre.log");
	}
	else if (mType == HTML) {
		openAsHTML("NxOgre.log.html");
	}
	else if (mType == PHPBB) {
		openAsPHPBB("NxOgre.log.phpbb.txt");
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

Log::~Log() {

	
	if (mType == TEXT)  {
		closeAsText();
	}
	else if (mType == HTML) {
		closeAsHTML();
	}
	else if (mType == PHPBB) {
		closeAsPHPBB();
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::report(const ErrorReport& report) {
	
	if (mType == TEXT) {
		reportAsText(report);
	}
	else if (mType == HTML) {
		reportAsHTML(report);
	}
	else if (mType == PHPBB) {
		reportAsPHPBB(report);
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::openAsHTML(const NxString& f) {
	mStream.open(f.c_str());
	mStream << 
		"<html>" << std::endl << 
		"	<head>" << std::endl << 
		"		<title>NxOgre Log - " << Nx_Version_Full << "</title>" << std::endl << 
		"			<style>" << std::endl << 
		"				body {background:#121212;color:white;font-family:Helvetica,sans-serif;font-size:80%;padding-top:1em;}" << std::endl << 
		"				h2 {font-size:145%;margin:0;padding:0;color:#999;}" << std::endl << 
		"				h2 small {padding-left:1em;}" << std::endl << 
		"				div.log {width:75%;margin:0 auto 2em auto;padding:1em 2em 2em 1em;border-left:2px solid #333;background-color:#181818;}" << std::endl << 
		"				div.conflict {border-left:2px solid hotpink;}" << std::endl <<
		"				div.error {border-left:2px solid red;}" << std::endl <<
		"				div.error h2{color:orangered}" << std::endl <<
		"				div.leak {border-left:2px solid orangered;}" << std::endl <<
		"				div.debug {border-left:2px solid #333;}" << std::endl <<
		"				div.debug h2 {font-size:125%;color:#444;}" << std::endl <<
		"				div.warning {border-left:2px solid SaddleBrown;}" << std::endl <<
		"				span.item{font-size:85%;background:red;}" << std::endl << 
		"				small {font-size:95%;color:#333;}" << std::endl << 
		"				div.et0{border-left:2px solid red;}" << std::endl << 
		"				div.et99{}" << std::endl << 
		"				pre {font-size:1.1em;padding:0.5em 0 0 0;margin:0;overflow:auto;}" << std::endl <<
		"				h1 {background: url(\"data:image/png;base64," <<
						"iVBORw0KGgoAAAANSUhEUgAAAPoAAABPCAMAAAD1NSjJAAAAGFBMVEUKCgr+/v4tLS1VVVWEhITY2Nitra3" <<
						"///+Y5SUzAAAACHRSTlP/////////AN6DvVkAAAAZdEVYdFNvZnR3YXJlAEFkb2JlIEltYWdlUmVhZHlxyW" <<
						"U8AAAF+0lEQVR42u2c2YKkIAxFYzb+/4/nQUGWhMWy26rq4W2qo3hYArmBgfBnC/xtdExl1BC71ZVqEIn4L" <<
						"ESE8Dw6bKnwwHi3WvxmIFbZjCLKBO+CvuHd6EC6dQvjm6DLzego27AovAX6Rrei8zZTBN8CXeBG9IxcmOh0" <<
						"p1hOf4Fn0WXs6RbRMXozQgDD6RPLzFj7eXSSoadbQwcZ+jGIlcKj6BAHPtyETjs5zLQPPYsepybfhK5T03h" <<
						"fA/RhdBgN+SX0fabrTS30w+jJL92CzpMjmSb2Uj+PHrTvcJfQZRJpH2v0NPrA062gH7MHJtuIn0YfeLol9G" <<
						"lbfcjPVejQXWdX0HHalsfBw2+gH07H6YR19BnPTTe4eAAAeBG96+lyS0/bgOMPs2tbbG2oN7msqqqcJI2yP" <<
						"jj/iUc8INpuHAEpvYm5lEcadOhEFIZXQHtNk6PXeRo9exGytCE9lD2S2gvVDYCBuImZ5WyeBj0OeR4NeDW7" <<
						"lWKL0CJ6ggK1Q3pp0cUIi3GoFSh66HHI4wDd3H2fP9L03ry0tD9YwEJXQxCQqgutQh66P+RLSzLMOHXSRXT" <<
						"IyFVVNRGpgU5RC0BMQpha5NmbUrca6LEpaYAeg9J2RcNwGZ2TsoEAAQLEsF6NXt9No7R5zHopVmllwt37Ay" <<
						"CxZs1jobthTGWJjZmejTG/Nc/Rj1dWkS60mgZZjk1PGyqbpRYI0EF3F/fakiuzfApcQ1cnxmcPvXTp555YP" <<
						"NGPko2J7i3ujVcoPR3k9V1CB9fLqI1emVLsis7SqnHzaKOj7ekay3LLVoSp8+h4PuZ/MdnoZLxKUt+hXx14" <<
						"6E4Y400NzbUJeAGd3YfqpZTMvonovbAx7Y0cdNvTGZbnxx7DHZYViGzAd75YLXQNDnontzFCtz2dYQlJ16l" <<
						"Yr6D3OostdO6ic0dIYB/d9HSWZfxwqBSHS+idaI8s9NoPo6oqF3PIGT89dMvTmZaHrMiV9XV0DZfRDffhSy" <<
						"MuuuXpTMt864nhJfRetIcWOl6ROsfohqfrjo/qo6+j0+Pohqfrj49ycjyKfmwz2SwyRm/DmPnxMZhuv4PeT" <<
						"er30Q8kgZE2R8bvn43eZGMGvU4fhC7QR68FG8cyBZBtGDAjUJ0bwt+Z6/sRrkGv14LNyMNr8+ij6LgoRvcW" <<
						"9+G6TjW6TqPDu6GX2RjTcq+G6+03TJzIKraDA3S6bV2fRI+jWV30mGqoQ6kqkBuij/zDBXR6Db0IY6zILSk" <<
						"zVVQZZjOtcB6rwNU9PF7Zw0+j557OjdeziBPbKTyTkuUwiNx4HZ1fRM89nafS5ELkslaB3VHVlyqw25yOoy" <<
						"EiIppBzzydJ1tjLh1yUf0YPVcl/UliC1QDdBspNssYPfN0jWUJW+nyk0ejuMnjof8Rs+g9RzMTvjR7utqy1" <<
						"ga5FWiHLr5YCfwcJS+h9xRZWEFPYYydfaHakBdWmJLW1+FlDb2zViS3OoOeIndayLml/uye9s8TQfYry8Tz" <<
						"LHrn/BaNFFlzyGthyW6m9fjtTB06R3NS6l+rnEadc9NtFT1OEC/Axll0KJLeYA/3zFCrp0SZidKxcCIiVpG" <<
						"tDfk0HX6IY+toIFlDj3ULn6eyAVIqWmEWvQyAIeuJWuanfMjTNlfICgNFlfk8Ns9r6MVZfFVVzZvaz7QOuj" <<
						"07SyPeWRrI0poj1YDM4L9qHVhE78oVFObRiy8CM7duenngEXw9r80TJUJhGd1vd6Gwgh60QPdccesDAJ2rX" <<
						"seZB2Mhqy9JCUO4gB6AjJpFY5X7FT8iGt04A6RYIAQgRO+Z3RCrZ5lVVY6iykzmtZD4CmYVEdF02K1Ch5lv" <<
						"Tgfwzkqzk3O/eg4dUpk2bhOk1+ttI7fPKK+gu0Hrf/T/6M/yMTPbt8O+Ht3nw29H9+/B3H876M3muq/szKq" <<
						"cH4vuXnqbl/U/Fd1Vdng6hfep6GCfbQXebp/qb7eux2RPvoWFGMzd+/8bvBt6kipE9vR4rjDce/vz7XZzNC" <<
						"XmfOdG1mPXu2/8vuEeHg1l55Qpvzx8ASRO13Tya37fj17qGj9WQfiz5R/SFale8MO0+QAAAABJRU5ErkJgg" <<
						"g==\") top center no-repeat;padding-top:85px;text-align:center;font-size:1.2em;}" << std::endl << 
		"				a#jump {position:absolute;top:0.5em;right:0.5em;color:white;text-decoration:none;}" << std::endl <<
		"				a#jump:hover {text-decoration:underline;}" << std::endl <<
		"				h5 {width:80%;margin:0 auto 2em auto;padding:1.25em 0 0.25em 0;border-bottom:2px solid red;text-align:center;}" << std::endl <<
		"				div#e {width:90%;margin:0 auto 2em auto;padding:1.25em 0 0 0;font-size:0.9em;text-align:center;}" << std::endl <<
		"				div#key {margin:0 auto 2em auto;display:table;font-size:0.75em;}" <<
		"				div#key span{display:table-cell;margin-left:24px;width:12px;height:1em;font-size:1px;border-left:32px solid #121212;border-right:8px solid #121212;}" <<
		"			</style>" << std::endl << 
		"	</head>" << std::endl << 
		"<body>" << std::endl <<
		"<h1>Log<br /><small>" << Nx_Version_Full << "</small></h1><a href=\"#bottom\" id=\"jump\">Jump to bottom</a>" << 
		"<div id=\"key\"><span style=\"background-color:#333;\">&nbsp;</span> Debug <span style=\"background-color:Indigo;\">&nbsp;</span> Warning" <<
		"<span style=\"background-color:hotpink;\">&nbsp;</span> Conflict" << 
		"<span style=\"background-color: red;\">&nbsp;</span> Error <span style=\"background-color: orangered;\">&nbsp;</span> Leaks</div>"
		<< std::endl;

	mStream.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::openAsText(const NxString& f) {
	mStream.open(f.c_str());
	mStream << Nx_Version_Full << std::endl;
	mStream.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::openAsPHPBB(const NxString& f) {
	mStream.open(f.c_str());
	mStream << "[b][u]**Log Begin**[/u][/b]" << std::endl
			<< "Versions: NxOgre(" << Nx_Version_Full << ")" 
			<< ", Ogre(" <<  Ogre::StringConverter::toString(OGRE_VERSION_MAJOR) + "." + Ogre::StringConverter::toString(OGRE_VERSION_MINOR) + "." + Ogre::StringConverter::toString(OGRE_VERSION_PATCH) + " '" + OGRE_VERSION_NAME + "')"
			<< ", PhysX(" << Ogre::StringConverter::toString(NX_SDK_VERSION_MAJOR) + "." + Ogre::StringConverter::toString(NX_SDK_VERSION_MINOR) + "." + Ogre::StringConverter::toString(NX_SDK_VERSION_BUGFIX) << ")" << std::endl;
	mStream.flush();
}
////////////////////////////////////////////////////////////////////////////////////////////////

std::string Log::encodeHTML(std::string text) {
	{
		std::string::size_type found_at = text.find( ">" );
		while( std::string::npos != found_at ) {
			text.replace( found_at, 1, "&gt;" );
			found_at = text.find( ">", found_at + 1 );
		}
	}
	{
		std::string::size_type found_at = text.find( "<" );
		while( std::string::npos != found_at ) {
			text.replace( found_at, 1, "&lt;" );
			found_at = text.find( "<", found_at + 1 );
		}
	}

	return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::reportAsHTML(const ErrorReport& r) {

	std::string message = encodeHTML(r.Message);
	std::string caller = encodeHTML(r.Caller);

	if (r.type == ErrorReport::ET_Leak) {
		mStream <<  "<div class=\"log leak\"><pre><code>" << message << "</code></pre></div>" << std::endl;
	}
	else if (r.type == ErrorReport::ET_FatalError) {
		mStream <<	"<div class=\"log error" << "\">" << std::endl <<
					"	<h2>" << caller << "<small>T" << r.second << " F" << r.frame << "</small></h2>" << std::endl <<
					"	<pre><code>" << message << "</code></pre>" << std::endl << 
					"</div>";
	}
	else if (r.type == ErrorReport::ET_Conflic) {
		mStream <<	"<div class=\"log conflict\">" << std::endl <<
					"	<h2>" << caller << "<small>T" << r.second << " F" << r.frame << "</small></h2>" << std::endl <<
					"	<pre><code>" << message << "</code></pre>" << std::endl << 
					"</div>";
	}
	else if (r.type == ErrorReport::ET_Debug) {
		mStream <<	"<div class=\"log debug\">" << std::endl <<
					"	<h2>" << caller << "<small>T" << r.second << " F" << r.frame << "</small></h2>" << std::endl <<
					"	<pre><code>" << message << "</code></pre>" << std::endl << 
					"</div>";
	}
	else if (r.type == ErrorReport::ET_Warning) {
		mStream <<	"<div class=\"log warning\">" << std::endl <<
					"	<h2>" << caller << "<small>T" << r.second << " F" << r.frame << "</small></h2>" << std::endl <<
					"	<pre><code>" << message << "</code></pre>" << std::endl << 
					"</div>";
	}
	else {
		mStream <<	"<div class=\"log\">" << std::endl <<
					"	<h2>" << caller << "<small>T" << r.second << " F" << r.frame << "</small></h2>" << std::endl <<
					"	<pre><code>" << message << "</code></pre>" << std::endl << 
					"</div>";
	}
	mStream.flush();

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::reportAsText(const ErrorReport& r) {
	mStream << "T" << r.second << "F" << r.frame << std::endl << r.Caller << std::endl << r.Message << std::endl;
	mStream.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::reportAsPHPBB(const ErrorReport& r) {
	mStream << "[b]T" << r.second << "F" << r.frame <<"[/b]" << std::endl << r.Caller << std::endl
			<< "[code]" << r.Message << "[/code]" << std::endl;
	mStream.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::closeAsHTML() {
	mStream << "<a name=\"bottom\"></a><h5>Log Complete</h5>";
#ifdef NX_DEBUG
	mStream << "<div id=\"e\">This is a HTML format of the NxOgre log. To make a copy of this for the Ogre forums or to work in plain text; Add either of these two params <code>log:phpbb</code> or <code>log:text</code> in the World constructor.</div>";
#endif
	mStream << "</body></html>";
	mStream.flush();
	mStream.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::closeAsText() {
	mStream.flush();
	mStream.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Log::closeAsPHPBB() {
	mStream << "[b][u]**Log End**[/u][/b]" << std::endl;
	mStream.flush();
	mStream.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
