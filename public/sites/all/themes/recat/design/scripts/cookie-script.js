	var cookieScripts = document.getElementsByTagName("script"),
    cookieScriptSrc = cookieScripts[cookieScripts.length-1].src,
	cookieQuery = null,
	cookieScriptPosition = "bottom",
	cookieScriptSource = "cookie-script.com",
	cookieScriptDomain = "",
	cookieScriptReadMore = "/privacy-and-cookies",
	cookieId="6bd265281b5d6ec2840e9c2298614c51",
	cookieScriptDebug = 0,
	cookieScriptCurrentUrl=window.location.href ,
	cookieScriptTitle= '<h4 id="cookiescript_header">This website uses cookies</h4>',
	cookieScriptDesc= "This website uses cookies to improve user experience. By using our website you consent to all cookies in accordance with our Cookie Policy.<br \/>",
	cookieScriptAccept='I agree',
	cookieScriptMore='Read more',
	cookieScriptCopyrights='I agree',
	cookieBackground='#EEE',
	cookieTextColor='#000',
	cookieScriptLoadJavaScript = function (d, b) {
        var c = document.getElementsByTagName("head")[0],
            a = document.createElement("script");
        a.type = "text/javascript", a.src = d, b != undefined && (a.onload = a.onreadystatechange = function () {
            (!a.readyState || /loaded|complete/.test(a.readyState)) && (a.onload = a.onreadystatechange = null, c && a.parentNode && c.removeChild(a), a = undefined, b())
        }), c.insertBefore(a, c.firstChild)
    }, 
	
	InjectCookieScript = function () {
				 
		cookieScriptCreateCookie = function (n, t, i) {
			if(window.location.protocol == 'https:')
				var cookieScriptSecureCookie=";secure";
			else
				var cookieScriptSecureCookie="";
			var u = "",
				r, f;
			i && (r = new Date, r.setTime(r.getTime() + i * 864e5), u = "; expires=" + r.toGMTString()), f = "", cookieScriptDomain != "" && (f = "; domain=" + cookieScriptDomain), document.cookie = n + "=" + t + u + f + "; path=/"+cookieScriptSecureCookie;
		}, 
		
		cookieScriptReadCookie = function (n) {
			for (var r = n + "=", u = document.cookie.split(";"), t, i = 0; i < u.length; i++) {
				for (t = u[i]; t.charAt(0) == " ";) t = t.substring(1, t.length);
				if (t.indexOf(r) == 0) return t.substring(r.length, t.length)
			}
			return null
		};
        cookieQuery(function () {
			/*if (window!=window.top) { cookieScriptWindow=window.parent.document }	
			else */
			cookieScriptWindow=window.document;
			cookieQuery("#cookiescript_injected",cookieScriptWindow).remove();
			cookieQuery("#cookiescript_overlay",cookieScriptWindow).remove();
			cookieQuery("#cookiescript_info_box",cookieScriptWindow).remove();
			
cookieScriptCurrentValue=cookieScriptReadCookie("cookiescriptaccept");
if(cookieScriptCurrentValue == "visit") return false; 
            cookieQuery("body",cookieScriptWindow).append('<div id="cookiescript_injected"><div id="cookiescript_wrapper">'+cookieScriptTitle+cookieScriptDesc+'<div id="cookiescript_buttons"><div id="cookiescript_accept">'+cookieScriptAccept+'</div><div id="cookiescript_readmore">'+cookieScriptMore+'</div></div><a href="//'+cookieScriptSource+'" target="_blank" id="cookiescript_link" style="display:block !important">Free cookie consent by cookie-script.com</a><div id="cookiescript_pixel"></div></div>');
            cookieQuery("#cookiescript_injected",cookieScriptWindow).css({
               "background-color": "#EEEEEE",
                "z-index": 999999,
                opacity: 1,
                position: "fixed",
                padding: "15px 0px 5px 0",
                width: "100%",
				left: 0, 
				"font-size": "13px",
				"font-weight": "normal",
                "text-align": "left",
				"letter-spacing":"normal",
                color: "#000000",
                "font-family": "Arial, sans-serif",
                display: "none",
				"-moz-box-shadow": "0px 0px 8px #000000",
				"-webkit-box-shadow": "0px 0px 8px #000000",
				"box-shadow": "0px 0px 8px #000000"
            });
			
			cookieQuery("#cookiescript_buttons",cookieScriptWindow).css({
				width: "186px",
				"margin":"0 auto",
				"font-size": "13px",
				"font-weight": "normal",
                "text-align": "center",
                "font-family": "Arial, sans-serif"
            });
			cookieQuery("#cookiescript_wrapper",cookieScriptWindow).css({
				width: "100%",
				"margin":"0 auto",
				"font-size": "13px",
				"font-weight": "normal",
                "text-align": "center",
                color: "#000000",
                "font-family": "Arial, sans-serif",
				"line-height": "18px",
				"letter-spacing":"normal"
            });
			
            if (cookieScriptPosition == "top") {
                cookieQuery("#cookiescript_injected",cookieScriptWindow).css("top", 0);
				
            } else {
                cookieQuery("#cookiescript_injected",cookieScriptWindow).css("bottom", 0);
            }
			
			cookieQuery("#cookiescript_injected h4#cookiescript_header",cookieScriptWindow).css({
                "background-color": "#EEEEEE",
                "z-index": 999999,
                padding: "0 0 7px 0",
                "text-align": "center",
                color: "#000000",
                "font-family": "Arial, sans-serif",
				 display: "block",
                "font-size": "15px",
				"font-weight": "bold",
                margin: "0"
            });
			
            cookieQuery("#cookiescript_injected span",cookieScriptWindow).css({
                display: "block",
                "font-size": "100%",
                margin: "5px 0"
            });
            cookieQuery("#cookiescript_injected a",cookieScriptWindow).css({
                "text-decoration": "underline",
                color: cookieTextColor
            }); 
           
			 cookieQuery("#cookiescript_injected a#cookiescript_link",cookieScriptWindow).css({
                "text-decoration": "none", 
                color: "#000000",
                "font-size": "85%",
                "text-decoration": "none",
                "float": "right",
				padding: "0px 10px 0 0"
                
            });
             cookieQuery("#cookiescript_injected div#cookiescript_accept,#cookiescript_injected div#cookiescript_readmore",cookieScriptWindow).css({
                "-webkit-border-radius": "5px",
                "-khtml-border-radius": "5px",
                "-moz-border-radius": "5px",
                "border-radius": "5px",
                border: 0,
                padding: "6px 10px",
                "font-weight": "bold",
				"font-size": "13px",
                cursor: "pointer",
                margin: "5px 0px",
				"-webkit-transition": "0.25s",
				"-moz-transition": "0.25s",
				"transition": "0.25s",
				"text-shadow": "rgb(0, 0, 0) 0px 0px 2px"
            });
            cookieQuery("#cookiescript_injected div#cookiescript_readmore",cookieScriptWindow).css({
                "background-color": "#697677",
				color: "#FFFFFF",
				"float": "right"
            });
			cookieQuery("#cookiescript_injected div#cookiescript_accept",cookieScriptWindow).css({
                "background-color": "#5BB75B",
				color: "#FFFFFF",
				"float": "left"
            });
			 
			cookieQuery("#cookiescript_injected div#cookiescript_pixel",cookieScriptWindow).css({
				"width": "1px",
				"height":"1px",
				"float": "left"
            });
			if(window._gaq){_gaq.push(['_trackEvent', 'Cookie-Script', 'Show', {'nonInteraction': 1}])}else if(window.ga){ga('send', 'event', 'Cookie-Script', 'Show', {'nonInteraction': 1});}			
			/*cookieQuery("#cookiescript_injected div#cookiescript_accept",cookieScriptWindow).hover( function(){
				cookieQuery(this).css('background-color', '#51a351');
			},function(){
				cookieQuery(this).css('background-color', '#5bb75b');
			});
			
			
			cookieQuery('#cookiescript_injected div#cookiescript_readmore').hover( function(){
				cookieQuery(this).css('background-color', '#697677');
			},function(){
				cookieQuery(this).css('background-color', '#7B8A8B');
			});
			*/
			
			
            cookieQuery("#cookiescript_injected",cookieScriptWindow).fadeIn(1000); 
			
			
			
			cookieQuery("#cookiescript_injected div#cookiescript_accept",cookieScriptWindow).click(function(){
				if(window._gaq){_gaq.push(['_trackEvent', 'Cookie-Script', 'Accept', {'nonInteraction': 1}])}else if(window.ga){ga('send', 'event', 'Cookie-Script', 'Accept', {'nonInteraction': 1});}				cookieScriptAcceptFunction();
			}); 
			
			cookieQuery("#cookiescript_injected div#cookiescript_readmore",cookieScriptWindow).click(function(){
				if(window._gaq){_gaq.push(['_trackEvent', 'Cookie-Script', 'Read more', {'nonInteraction': 1}])}else if(window.ga){ga('send', 'event', 'Cookie-Script', 'Read more', {'nonInteraction': 1});}				
								window.open(cookieScriptReadMore, '_self');
				return false;
								
			})
			
			cookieQuery("#cookiescript_overlay",cookieScriptWindow).click(function(){
				cookieScriptDoBox("hide");
			});
			
			cookieQuery("#cookiescript_info_close",cookieScriptWindow).click(function(){
				cookieScriptDoBox("hide");
			});
			
			document.onkeydown = function(evt) {
				evt = evt || window.event;
				if (evt.keyCode == 27) {
					cookieScriptDoBox("hide");
				}
			};
			cookieScriptAltIframe();
			        });  
		function cookieScriptAltIframe(){
			cookieQuery('iframe'+'[data-cookiescript="accepted"]').not(':has([src])').each(function () 
			{
				var ifrm = this;
				
				ifrm = (ifrm.contentWindow) ? ifrm.contentWindow : (ifrm.contentDocument.document) ? ifrm.contentDocument.document : ifrm.contentDocument;
				
				ifrm.document.open();
				ifrm.document.write(cookieQuery(this).attr("alt"));
				ifrm.document.close();
			});
		}
		function cookieScriptAcceptFunction(){
			if(typeof cookieScriptScrollfired !== 'undefined') return false;
			cookieQuery("#cookiescript_injected",cookieScriptWindow).fadeOut(200);
			cookieScriptCreateCookie("cookiescriptaccept", "visit", 60);
			cookieScriptDoBox("hide");
			cookieScriptAllowJS();
		}
		function cookieScriptDoBox(action) {	
			if (action == "show") {
				cookieQuery("#cookiescript_overlay",cookieScriptWindow).show();
				cookieQuery("#cookiescript_info_box",cookieScriptWindow).show();
			} else if (action == "hide") {
				cookieQuery("#cookiescript_overlay",cookieScriptWindow).hide();
				cookieQuery("#cookiescript_info_box",cookieScriptWindow).hide();
			}
		}
		
		function cookieScriptAllowJS(){
			cookieQuery('img'+'[data-cookiescript="accepted"]').each(function () {
				cookieQuery(this).attr('src', cookieQuery(this).attr("data-src"));
       		});
			cookieQuery('script'+'[type="text/plain"]'+'[data-cookiescript="accepted"]').each(function () {
				if (cookieQuery(this).attr('src')) {
					cookieQuery(this).after('<script type="text/javascript" src="' + cookieQuery(this).attr("src") + '"><\/script>')
				} else {
					cookieQuery(this).after('<script type="text/javascript">' + cookieQuery(this).html() + '<\/script>')
				}
				cookieQuery(this).empty();
       		});
			cookieQuery('iframe'+'[data-cookiescript="accepted"]').each(function () {
				cookieQuery(this).attr('src', cookieQuery(this).attr("data-src"));
       		});
			cookieQuery('embed'+'[data-cookiescript="accepted"]').each(function () {
				cookieQuery(this).replaceWith(cookieQuery(this).attr('src',cookieQuery(this).attr("data-src"))[0].outerHTML);				
       		});
			cookieQuery('object'+'[data-cookiescript="accepted"]').each(function () {
				cookieQuery(this).replaceWith(cookieQuery(this).attr('data',cookieQuery(this).attr("data-data"))[0].outerHTML);				
       		});
			cookieQuery('link'+'[data-cookiescript="accepted"]').each(function () {
				cookieQuery(this).attr('href', cookieQuery(this).attr("data-href"));
			});
		}
		if(cookieScriptReadCookie('cookiescriptaccept') == 'visit' || cookieScriptReadCookie('cookiescriptaccept') == 'shown') 
			cookieScriptAllowJS();
			
    };

window.jQuery && jQuery.fn && /^(1\.[8-9]|2\.[0-9])/.test(jQuery.fn.jquery) ? (cookieScriptDebug && window.console && console.log("Using existing jQuery version " + jQuery.fn.jquery), cookieQuery = window.jQuery, InjectCookieScript()) : (cookieScriptDebug && window.console && console.log("Loading jQuery 1.8.1 from ajax.googleapis.com"), cookieScriptLoadJavaScript(("https:" == document.location.protocol ? "https://" : "http://") + "ajax.googleapis.com/ajax/libs/jquery/1.8.1/jquery.min.js", function () {cookieQuery = jQuery.noConflict(!0), InjectCookieScript();}));