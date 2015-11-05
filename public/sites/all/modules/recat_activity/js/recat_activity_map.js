(function ($) {
    Drupal.behaviors.recatActivityMap = {
        attach: function (context, settings) {
            var el = $('#activityMap');

            if (!settings.recatActivityMap
                || !settings.recatActivityMap.terms) {
                el.remove();
                return;
            }

            var selectedRegions = [],
                markers = [];

            for (var i = 0, len = settings.recatActivityMap.terms.length; i < len; i += 1) {
                selectedRegions.push(settings.recatActivityMap.terms[i].countryCode);
            }

            jvm.Map.prototype.getRegionCenter = function(region) {
                if (typeof region === 'string') {
                    region = this.regions[region.toUpperCase()];
                }

                var bbox = region.element.shape.getBBox(),
                    xcoord = (((bbox.x + bbox.width / 2) + map.transX) * map.scale),
                    ycoord = (((bbox.y + bbox.height / 2) + map.transY) * map.scale);

                return this.pointToLatLng(xcoord, ycoord);
            };

            var map = new jvm.Map({
                    container: el,
                    map: 'europe_mill',
                    backgroundColor: '#ffffff',
                    zoomOnScroll: false,
                    panOnDrag: false,
                    selectedRegions: selectedRegions,
                    regionStyle: {
                        initial: {
                            fill: '#cad3df',
                            'fill-opacity': 1,
                            stroke: '#ffffff',
                            'stroke-width': 1
                        },
                        hover: {
                            'fill-opacity': 1,
                            cursor: 'default'
                        },
                        selected: {
                            fill: '#9dbf16'
                        }
                    }
                });

            map.setFocus({ scale: 1.8, x: 0.8, y: 0.6 });

            for (var i = 0, len = settings.recatActivityMap.terms.length; i < len; i += 1) {
                var latLng = map.getRegionCenter(settings.recatActivityMap.terms[i].countryCode),
                    pos = map.latLngToPoint(latLng.lat, latLng.lng);

                markers.push('<a href="%href" class="marker sprite-before" data-countrycode="%countryCode" style="top: %toppx; left: %leftpx;">%count</a>'
                    .replace('%href', settings.recatActivityMap.terms[i].url)
                    .replace('%countryCode', settings.recatActivityMap.terms[i].countryCode)
                    .replace('%top', pos.y)
                    .replace('%left', pos.x)
                    .replace('%count', settings.recatActivityMap.terms[i].count));
            }

            el.find('.loader').remove();
            el.append($('<div class="markers" />').html(markers.join('')));
        }
    };
}) (jQuery);