# node-efsw

Watching your filesystem events just like [fsevents](https://www.npmjs.com/package/fsevents). But **node-efsw** is cross-platform.

> Node.js binding for [efsw](https://github.com/SpartanJ/efsw).

## Installation

```sh
$ npm install --save efsw
```

## Usage

```js
const Watcher = require("efsw").Watcher;

const watcher = new Watcher("/tmp");
watcher.on("change", function(filename, info) {
    console.log(filename, info);
});
watcher.on("error", function(err) {
    console.error(err);
});

watcher.start();
watcher.stop();
```

## API

### Constructor

```js
new Watcher(dir);
```

#### Parameters

+ `dir`: the directory to be watched.

### `start()`

Start the watcher. It may occur some error events, eg. the directory is not exist.

### `stop()`

Stop the watcher.

### Events

#### `.on("change", function(filename, info) {})`

This event will be emitted when a filesystem event occurred, eg. a file added.

##### Callback Parameters

+ `filename`: the absolute filename;
+ `info`: the event information object;
    - `info.dir`: the base directory that this watcher is watching;
    - `info.action`: the event type:
        * `"ADD"`
        * `"DELETE"`
        * `"MODIFIED"`
        * `"MOVED"`
    - `info.relative`: the relative filename that based from `info.dir`;
    - `info.old`: if the event is `"MOVED"`, this column indicates the old filename;
    - `info.oldRelative`: similar with `info.relative`. If the event is `"MOVED"`, this column indicates the old filename.

#### `.on("error", function(err) {})`

This event will be emitted when an error occurred.

##### Callback Parameters

+ `err`: the error object.

## Contribution

You're welcome to make pull request.
