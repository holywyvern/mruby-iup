# mruby-iup   [![Build Status](https://travis-ci.org/holywyvern/mruby-iup.svg?branch=master)](https://travis-ci.org/holywyvern/mruby-iup)
Example class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'holywyvern/mruby-iup'
end
```
## example
```ruby
p Example.hi
#=> "hi!!"
t = Example.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the Apache-2.0 License:
- see LICENSE file
