module.exports = function(grunt) {
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        uglify: {
            options: {
                banner: '/*! <%= pkg.name %> <%= grunt.template.today("HH:nn:ss dd-mm-yyyy") %> */\n'
            },
            recatLibrary: {
                files: {
                    '<%= pkg.drupalPath %>/recat_library/js/recat_library.min.js': [
                        'recat-library/src/recatLibrary.js',
                        'recat-library/**/*.js'
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