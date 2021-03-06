module.exports = function(grunt) {
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        uglify: {
            options: {
                banner: '/*! <%= pkg.name %> <%= grunt.template.today("yyyy-mm-dd HH:MM:ss") %> */\n'
            },
            recatLibrary: {
                files: {
                    '<%= pkg.drupalPath %>/recat_library/js/recat_library.min.js': [
                        'recat-library/src/recatLibrary.js',
                        'recat-library/**/*.js'
                    ]
                }
            },
            recatActivity: {
                files: {
                    '<%= pkg.drupalPath %>/recat_activity/js/recat_activity.min.js': [
                        'recat-activity/src/recatActivity.js',
                        'recat-activity/**/*.js'
                    ]
                }
            }
        },
        watch: {
            scripts: {
                files: ['**/*.js'],
                tasks: ['uglify'],
                options: {
                    spawn: false
                }
            }
        }
    });

    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-watch');

    grunt.registerTask('default', ['uglify', 'watch']);
    grunt.registerTask('once', ['uglify']);

};