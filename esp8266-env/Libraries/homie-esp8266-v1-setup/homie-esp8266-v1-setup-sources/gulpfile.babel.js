'use strict';

import gulp from 'gulp';
import plumber from 'gulp-plumber'; // help to avoid crash if error in a task
import newer from 'gulp-newer';
import watch from 'gulp-watch'; // gulp.watch doesn't detect new files
import smoosher from 'gulp-smoosher';
import zopfli from 'gulp-zopfli';
import rename from 'gulp-rename';
import del from 'del'; // delete files
import imagemin from 'gulp-imagemin';
import runSequence from 'run-sequence';
import notifier from 'node-notifier';
import uglify from 'gulp-uglify';
import browserify from 'browserify';
import envify from 'envify/custom';
import babelify from 'babelify';
import source from 'vinyl-source-stream'; // helper for browserify text stream to gulp pipeline
import buffer from 'vinyl-buffer'; // helper for browserify

let errored = false;
let errorHandler = function (task) {
  return function (error) {
    errored = true;
    console.log(`Error in ${task}: ${error.message}`);
    notifier.notify({
      title: `Error in ${task}`,
      message: error.message
    });
  };
};

//  ################
//  # Entry points #
//  ################

gulp.task('dev', ['buildpublic:dev'], function (done) {
  watch('./app/assets/**/*', function (vinyl) {
    console.log(`${vinyl.path} was ${vinyl.event}, piping to public/...`);
    runSequence('assets');
  });

  watch('./app/vendor/**/*', function (vinyl) {
    console.log(`${vinyl.path} was ${vinyl.event}, piping to public/vendor/...`);
    runSequence('vendor');
  });

  watch('./app/js/**/*.js', function (vinyl) {
    console.log(`${vinyl.path} was '${vinyl.event}', running Babel...`);
    runSequence('es6-7:dev');
  });
});

gulp.task('dist', function (done) {
  runSequence('buildpublic:dist', 'builduigz', function () {
    if (errored) {
      console.log('Distribution failed');
      process.exit(-1);
    }

    done();
  });
});

//  ####################
//  # public directory #
//  ####################

gulp.task('builduigz', function () {
  return gulp.src('index.html')
    .pipe(plumber(errorHandler('buildpublic:imagemin')))
    .pipe(smoosher())
    .pipe(rename('ui_bundle'))
    .pipe(zopfli())
    .pipe(gulp.dest('./'));
});

gulp.task('buildpublic:dist', function (done) {
  runSequence(
    'buildpublic:clear',
    ['assets', 'vendor', 'es6-7:dist'],
    'buildpublic:imagemin',
  done);
});

gulp.task('buildpublic:dev', function (done) {
  runSequence(
    'buildpublic:clear',
    ['assets', 'vendor', 'es6-7:dev'],
  done);
});

gulp.task('buildpublic:clear', function (done) {
  del(['./public/**/*']).then(function () {
    done();
  });
});

gulp.task('buildpublic:imagemin', function () {
  return gulp.src('./public/img/**/*.{png,jpg,gif,svg}', {
    base: './public'
  })
    .pipe(plumber(errorHandler('buildpublic:imagemin')))
    .pipe(imagemin({
      progressive: true
    }))
    .pipe(gulp.dest('./public'));
});

// Babel

let es67 = (prod = false) => {
  const babelPlugins = [];
  if (prod) babelPlugins.push(['module-alias', [{ 'src': 'npm:preact-compat', 'expose': 'react' }, { 'src': 'npm:preact-compat', 'expose': 'react-dom' }]]);
  return browserify({ entries: './app/js/app.js', debug: false }) // debug for sourcemaps
  .transform(babelify, { presets: ['es2015', 'stage-3', 'react'], plugins: babelPlugins });
};

gulp.task('es6-7:dev', function () {
  return es67()
    .bundle()
    .on('error', function (error) {
      errorHandler('es6-7')(error);
      this.emit('end');
    }) // Don't crash if failed, plumber doesn't work with browserify
    .pipe(source('bundle.min.js'))
    .pipe(buffer())
    .pipe(gulp.dest('./public/js'));
});

gulp.task('es6-7:dist', function () {
  return es67(true)
    .transform(envify({ NODE_ENV: 'production' }), { global: true }) // global: act on node_modules (here react prod mode)
    .bundle()
    .on('error', function (error) {
      errorHandler('es6-7')(error);
      this.emit('end');
    }) // Don't crash if failed, plumber doesn't work with browserify
    .pipe(source('bundle.min.js'))
    .pipe(buffer())
    .pipe(uglify())
    .pipe(gulp.dest('./public/js/'));
});

// assets and vendor

gulp.task('assets', function () {
  return gulp.src('./app/assets/**/*', {
    base: './app/assets'
  })
    .pipe(plumber(errorHandler('assets')))
    .pipe(newer('./public'))
    .pipe(gulp.dest('./public'));
});

gulp.task('vendor', function () {
  return gulp.src('./app/vendor/**/*', {
    base: './app'
  })
    .pipe(plumber(errorHandler('vendor')))
    .pipe(newer('./public'))
    .pipe(gulp.dest('./public'));
});
