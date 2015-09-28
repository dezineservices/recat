(function ($) {
    Drupal.behaviors.hsEnhancements = {
        attach: function (context, settings) {
            this.datepicker($('input.datepicker', context));
        },
        datepicker: function(inputs) {
            if (!inputs.length) {
                return;
            }

            var i = 0, len = inputs.length, dateSplit = [], startDate = '',
                el = null, options = null, enabledDates = [];

            for(; i < len; ++i) {
                startDate = '';
                el = inputs[i];
                options = el.getAttribute('data-datepicker').split('|');
                enabledDates = this.getDaysBetweenDates(options[0] * 1000, options[1] * 1000);

                dateSplit = enabledDates[0].split(' ');
                startDate += dateSplit[0].length === 2
                    ? dateSplit[0]
                    : '0' + dateSplit[0];

                startDate += '/' + (dateSplit[1].length === 2
                    ? dateSplit[1]
                    : '0' + dateSplit[1]);

                startDate += '/' + dateSplit[2];

                $(el).Zebra_DatePicker({
                    always_show_clear: false,
                    lang_clear_date: '',
                    format: el.getAttribute('data-format'),
                    offset: [10, 25],
                    start_date: options[2],
                    disabled_dates: ['* * * *'],
                    select_other_months: true,
                    show_icon: false,
                    show_select_today: Drupal.settings.hsEnhancements.datepicker.today,
                    enabled_dates: enabledDates,
                    months: Drupal.settings.hsEnhancements.datepicker.months,
                    months_abbr: Drupal.settings.hsEnhancements.datepicker.months_abbr,
                    days: Drupal.settings.hsEnhancements.datepicker.days,
                    days_abbr: Drupal.settings.hsEnhancements.datepicker.days_abbr,
                    onSelect: function(date, dateFormatted, dateObject, el) {
                        dateObject.setHours(0);
                        el.trigger('date_change', dateObject);
                    }
                });
            }

            return true;
        },
        getDaysBetweenDates: function(start, end) {
            var startDate = new Date(), endDate = new Date(),
                newDate = null, dates = [];

            start = start ? start : new Date().getTime();
            end = end ? end : start + (60 * 60 * 24 * 60 * 1000);

            startDate.setTime(start);
            endDate.setTime(end);

            while(startDate < endDate){
                newDate = new Date(startDate.setDate(startDate.getDate() + 1));

                dates.push(startDate.getDate() + ' ' + (startDate.getMonth() + 1) + ' ' + startDate.getFullYear());
                startDate = new Date(newDate);
            }

            return dates;
        }
    };

}) (jQuery);