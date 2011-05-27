# -*- ruby -*-

require 'rake/clean'
require 'rake/testtask'
require 'rbconfig'

begin
  gem "rdoc"
  require "rdoc/task"
rescue Gem::LoadError
  warn "RDoc 2.4.2+ is required to build documentation"
end

require "rubygems/package_task"


ARCH = Config::CONFIG['arch']
SO_EXT = (ARCH =~ /darwin/) ? "bundle" : "so"
SO_FILE = "lib/#{ARCH}/cm17a_api.#{SO_EXT}"

MAKE = (ARCH =~ /win32/) ? 'nmake' : 'make'

CLOBBER.include(
  'ext/cm17a_api/*.o',
  'ext/cm17a_api/*.obj',
  'ext/cm17a_api/*.def',
  'ext/cm17a_api/*.exp',
  'ext/cm17a_api/*.lib',
  'ext/cm17a_api/*.pdb',
  'ext/cm17a_api/*.so',
  'ext/cm17a_api/*.bundle',
  'ext/cm17a_api/Makefile',
  'ext/cm17a_api/MANIFEST',
  'lib/*-*',
  'lib/*.so',
  'lib/*.bundle',
  '**/*.log'
  )

desc "Default task (run tests)"
task :default => :test

desc "Run the unit tests"
Rake::TestTask.new do |t|
  t.test_files = FileList['test/test_*.rb']
  t.libs << "lib/#{ARCH}"
end
task :test => [SO_FILE]

task :xtest => [SO_FILE] do
  test_files = FileList['test/test_*.rb']
  ruby "-Ilib", "-Ilib/#{ARCH}",  '-e0',  *test_files.collect { |fn| "-r#{fn}" }
end

task :compile => [SO_FILE]

directory File.dirname(SO_FILE)

file SO_FILE => ["ext/cm17a_api/cm17a_api.#{SO_EXT}", File.dirname(SO_FILE)] do |t|
  cp t.prerequisites.first, SO_FILE
end

file "ext/cm17a_api/Makefile" => ["ext/cm17a_api/extconf.rb"] do
  Dir.chdir("ext/cm17a_api") do ruby "extconf.rb" end
end

CM17A_FILES = FileList[
  'ext/cm17a_api/*.c',
  'ext/cm17a_api/*.h',
  'ext/cm17a_api/extconf.rb',
  'ext/cm17a_api/Makefile',
  'ext/cm17a_api/MANIFEST',
] 

file "ext/cm17a_api/cm17a_api.#{SO_EXT}" => CM17A_FILES do
  Dir.chdir("ext/cm17a_api") do
    sh MAKE
  end
end


# RDoc Documentation -------------------------------------------------

# Choose template, prefer the jamis template if it is available

if defined?(RDoc::Task) then
  def choose_rdoc_template
    libdir = Config::CONFIG['rubylibdir']
    templates = Dir[File.join(libdir, 'rdoc/**/jamis.rb')]
    templates.empty? ? 'html': 'jamis'
  end

  RDoc::Task.new do |rdoc|
    rdoc.rdoc_dir = 'html'
    rdoc.template = choose_rdoc_template
    rdoc.title    = "Ruby X10 Software"
    rdoc.options << '--line-numbers' << '--inline-source' << '--main' << 'README'
    rdoc.rdoc_files.include(
      'lib/**/*.rb',
      'doc/*.rdoc',
      'ext/**/cm17a_api.c',
      'README',
      'MIT-LICENSE')
    rdoc.rdoc_files.exclude('lib/**/other.rb', 'lib/**/bottlerocket.rb')
  end
end

# Packaging Tasks ----------------------------------------------------

file "ext/cm17a_api/MANIFEST" do |t|
  open(t.name, "w") do |f|
    f.puts Dir["ext/cm17a_api/*"].select { |fn|
      fn =~ /\.(h|c|rb)$/
    }.collect { |fn|
      File.basename(fn)
    }
  end
end

PKG_VERSION = '1.0.1'
PKG_FILES = FileList[
  'Rakefile',
  'setup.rb',
  'test/*.rb',
  'lib/**/*.rb',
  'ext/cm17a_api/MANIFEST',
  'ext/cm17a_api/*.h',
  'ext/cm17a_api/*.c',
  'ext/cm17a_api/*.rb',
]

if ! defined?(Gem)
  puts "Package Target requires RubyGEMs"
else
  spec = Gem::Specification.new do |s|
    
    #### Basic information.

    s.name = 'x10-cm17a'
    s.version = PKG_VERSION
    s.summary = "Ruby based X10 CM17A Firecracker Controller"
    s.description = <<-EOF
    EOF

    s.files = PKG_FILES.to_a

    #### C code extensions.

    s.extensions << "ext/cm17a_api/extconf.rb"

    #### Load-time details: library and application (you will need one or both).

    s.require_path = 'lib'                         # Use these for libraries.

    #### Documentation and testing.

    s.has_rdoc = true
#    s.extra_rdoc_files = rd.rdoc_files.reject { |fn| fn =~ /\.rb$/ }.to_a
#     s.rdoc_options <<
#       '--title' <<  'Rake -- Ruby Make' <<
#       '--main' << 'README' <<
#       '--line-numbers'

    #### Author and project details.

    s.author = "Jim Weirich"
    s.email = "jim@weirichhouse.org"
#    s.homepage = "http://rake.rubyforge.org"
  end

  Gem::PackageTask.new(spec) do |pkg|
    pkg.need_zip = true
    pkg.need_tar = true
  end
end


# Publishing Tasks ---------------------------------------------------


task :publish_on_osb => ["pkg/cm17a-#{PKG_VERSION}.tgz", "pkg/cm17a-#{PKG_VERSION}.gem"] do |t|
  sh %{ssh umlcoop mkdir -p "htdocs/software/cm17a"}
  sh %{scp -rq #{t.prerequisites.join(' ')} umlcoop:htdocs/software/cm17a}
end

# Optional publish task for Rake

require 'rake/contrib/sshpublisher'
require 'rake/contrib/rubyforgepublisher'

publisher = Rake::CompositePublisher.new
publisher.add Rake::RubyForgePublisher.new('x10-cm17a', 'jimweirich')
publisher.add Rake::SshFilePublisher.new(
  'umlcoop',
  'htdocs/software/x10-cm17a',
  '.',
  'x10-cm17a.blurb')

desc "Publish the Documentation to RubyForge."
task :publish => [:rdoc] do
  publisher.upload
end
