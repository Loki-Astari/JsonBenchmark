<html>
<head>
<link rel="stylesheet" href="combine.css">
<script src="combine.js"></script>
<!--
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap-theme.min.css">
<script src="https://code.jquery.com/jquery-1.8.2.js"></script>
<script src="https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization','version':'1','packages':['corechart','table']}]}"></script>
<script src="https://jquery-csv.googlecode.com/git/src/jquery.csv.js"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js"></script>
-->
<script>
$(function() {
  var csv = $('#textInput').val();
  var dt = google.visualization.arrayToDataTable($.csv.toArrays(csv));

  function sortCaseInsensitive(dt, column) {
    for (var row = 0; row < dt.getNumberOfRows(); row++) {
      var s = dt.getValue(row, column);
      dt.setValue(row, column, s.toUpperCase());
      dt.setFormattedValue(row, column, s);
    }
    dt.sort(column);
  }

  addSection("0. Overall");

  var overallDiv = document.createElement("div");
  overallDiv.className = "chart";
  $(overallDiv).data("filename", "overall" + "_" + "Result");
  $("#main").append(overallDiv);

  // Per type sections
  var types = dt.getDistinctValues(0);
  var overallDt, overallKeyRowMap = {}, overallKeyCount = {};
  for (var i in types) {
    var type = types[i];
    addSection(type);

    var view = new google.visualization.DataView(dt);
    view.setRows(view.getFilteredRows([{column: 0, value: type}]));

    var percentdt = google.visualization.data.group(
      view,
      [1],
      [{column: 3, aggregation: percentCalc, type: 'number'}]
    );
    sortCaseInsensitive(percentdt, 0);

    function percentCalc(a) {
      var sum = 0;
      for (var i = 0; i < a.length; i++)
        if (a[i].trim() == 'true')
          sum++;
      return sum / a.length;
    }

    var perecentFormatter = new google.visualization.NumberFormat({pattern:'###%'});
    perecentFormatter.format(percentdt, 1);

    drawBarChart(type, percentdt.clone());

    // Accumulate perecentage for overall
    if (overallDt == null) {
      overallDt = percentdt.clone();
      for (var row = 0; row < overallDt.getNumberOfRows(); row++) {
        var key = overallDt.getValue(row, 0);
        overallKeyRowMap[key] = row;
        overallKeyCount[key] = 1;
      }
    }
    else {
      for (var row = 0; row < percentdt.getNumberOfRows(); row++) {
        var key = percentdt.getValue(row, 0);
        var result = percentdt.getValue(row, 1);
        var r = overallKeyRowMap[key];
        if (r != null) {
          overallDt.setValue(r, 1, overallDt.getValue(r, 1) + result);
          overallKeyCount[key]++;
        }
        else {
          overallKeyRowMap[key] = overallDt.addRow([key, result]);
          overallKeyCount[key] = 1;
        }
      }
    }

    view.setColumns([ 2, 1, {calc:stringToBoolean, type:'string', label:''} ]);
    
    function stringToBoolean(dt, row) {
      var v = dt.getValue(row, 3);
      if (dt.getValue(row, 3).trim() == 'true')
        return '<span class="glyphicon glyphicon-ok text-success"></span>';
      else
        return '<span class="glyphicon glyphicon-remove text-danger"></span>';
    }

    drawTable(type, pivotTable(view));
  }

  {
    // average results
    for (var row = 0; row < overallDt.getNumberOfRows(); row++)
      overallDt.setValue(row, 1, overallDt.getValue(row, 1) / overallKeyCount[overallDt.getValue(row, 0)]);

    var perecentFormatter = new google.visualization.NumberFormat({pattern:'###%'});
    perecentFormatter.format(overallDt, 1);
    drawBarChart("0. Overall", overallDt, overallDiv)
  }

  $(".chart").each(function() {
    var chart = $(this);
    var d = $("#downloadDD").clone().css("display", "");
    $('li a', d).each(function() {
          $(this).click(function() {
              var svg = chart[0].getElementsByTagName('svg')[0].parentNode.innerHTML;
              svg=sanitize(svg);
              $('#imageFilename').val($("#title").html() + "_" + chart.data("filename"));
              $('#imageGetFormTYPE').val($(this).attr('dltype'));
              $('#imageGetFormSVG').val(svg);
              $('#imageGetForm').submit();
          });
      });   
    $(this).after(d);
  });

  // Add configurations
  var thisConfig = <?="\"".basename($argv[1], '.'.pathinfo($argv[1], PATHINFO_EXTENSION)).'.'.$argv[2]."\""?>;
  var configurations = [ "Conformance.osx", "Conformance.linux", "Performance.osx", "Performance.linux", "Stats"];

  for (var i in configurations) {
    var c = configurations[i];
    $("#benchmark").append($("<li>", {class : (c == thisConfig ? "active" : "")}).append($("<a>", {href: c + ".html"}).append(c)));
  }
});

function pivotTable(src) {
  var dst = new google.visualization.DataTable();
  
  // Add columns
  var key = src.getDistinctValues(1);
  var keyColumnMap = {};
  dst.addColumn(src.getColumnType(0), src.getColumnLabel(0));  
  for (var k in key)
    keyColumnMap[key[k]] = dst.addColumn(src.getColumnType(2), key[k]);

  // Add rows
  var pivot = src.getDistinctValues(0);
  var pivotRowMap = {};  
  for (var p in pivot)
    dst.setValue(pivotRowMap[[pivot[p]]] = dst.addRow(), 0, pivot[p]);

  // Fill cells
  for (var row = 0; row < src.getNumberOfRows(); row++)
    dst.setValue(
      pivotRowMap[src.getValue(row, 0)],
      keyColumnMap[src.getValue(row, 1)],
      src.getValue(row, 2));

  return dst;
}

function addSection(name) {
  $("#main").append(
    $("<a>", {"name": name}),
    $("<h2>", {style: "padding-top: 70px; margin-top: -70px;"}).append(name)
  );
  $("#section").append($("<li>").append($("<a>", {href: "#" + name}).append(name)));
}

function addSubsection(name) {
  $("#main").append(
    $("<h3>", {style: "padding-top: 70px; margin-top: -70px;"}).append(name)
  );
}

function drawTable(type, data) {
  var div = document.createElement("div");
  div.className = "tablechart";
  $("#main").append(div);
  var table = new google.visualization.Table(div);
  table.draw(data, { allowHtml: true});
}

function drawBarChart(type, data, div) {
  // Using same colors as in series
  var colors = ["#3366cc","#dc3912","#ff9900","#109618","#990099","#0099c6","#dd4477","#66aa00","#b82e2e","#316395","#994499","#22aa99","#aaaa11","#6633cc","#e67300","#8b0707","#651067","#329262","#5574a6","#3b3eac","#b77322","#16d620","#b91383","#f4359e","#9c5935","#a9c413","#2a778d","#668d1c","#bea413","#0c5922","#743411","#3366cc","#dc3912","#ff9900","#109618","#990099","#0099c6","#dd4477","#66aa00","#b82e2e","#316395","#994499","#22aa99","#aaaa11","#6633cc","#e67300","#8b0707","#651067","#329262","#5574a6","#3b3eac","#b77322","#16d620","#b91383","#f4359e","#9c5935","#a9c413","#2a778d","#668d1c","#bea413","#0c5922","#743411"];
  var h = data.getNumberOfRows() * 12;
  var options = { 
    title: type,
    chartArea: {left: '20%', width: '70%', height: h },
    width: 800,
    height: h + 100,
    fontSize: 10,
    bar: {groupWidth: "80%"},
    hAxis: { title: data.getColumnLabel(1), format: 'percent' },
    legend: { position: "none" },
  };

  data.addColumn({ type: "string", role: "style" })
  data.addColumn({ type: "number", role: "annotation" });
  for (var rowIndex = 0; rowIndex < data.getNumberOfRows(); rowIndex++) {
    data.setValue(rowIndex, 2, colors[rowIndex]);
    data.setValue(rowIndex, 3, data.getValue(rowIndex, 1));
  }

  var perecentFormatter = new google.visualization.NumberFormat({pattern:'###%'});
  perecentFormatter.format(data, 3);

  if (div == null) {
    div = document.createElement("div");
    div.className = "chart";
    $(div).data("filename", type + "_" + data.getColumnLabel(1));
    $("#main").append(div);
  }
  var chart = new google.visualization.BarChart(div);
  chart.draw(data, options);
}

// https://jsfiddle.net/P6XXM/
function sanitize(svg) {
    svg = svg
        .replace(/\<svg/,'<svg xmlns="http://www.w3.org/2000/svg" version="1.1"')
        .replace(/zIndex="[^"]+"/g, '')
        .replace(/isShadow="[^"]+"/g, '')
        .replace(/symbolName="[^"]+"/g, '')
        .replace(/jQuery[0-9]+="[^"]+"/g, '')
        .replace(/isTracker="[^"]+"/g, '')
        .replace(/url\([^#]+#/g, 'url(#')
        .replace('<svg xmlns:xlink="http://www.w3.org/1999/xlink" ', '<svg ')
        .replace(/ href=/g, ' xlink:href=')
    /*.replace(/preserveAspectRatio="none">/g, 'preserveAspectRatio="none"/>')*/
    /* This fails in IE < 8
    .replace(/([0-9]+)\.([0-9]+)/g, function(s1, s2, s3) { // round off to save weight
    return s2 +'.'+ s3[0];
    })*/

    // IE specific
        .replace(/id=([^" >]+)/g, 'id="$1"')
        .replace(/class=([^" ]+)/g, 'class="$1"')
        .replace(/ transform /g, ' ')
        .replace(/:(path|rect)/g, '$1')
        .replace(/<img ([^>]*)>/gi, '<image $1 />')
        .replace(/<\/image>/g, '') // remove closing tags for images as they'll never have any content
        .replace(/<image ([^>]*)([^\/])>/gi, '<image $1$2 />') // closes image tags for firefox
        .replace(/width=(\d+)/g, 'width="$1"')
        .replace(/height=(\d+)/g, 'height="$1"')
        .replace(/hc-svg-href="/g, 'xlink:href="')
        .replace(/style="([^"]+)"/g, function (s) {
            return s.toLowerCase();
        });

    // IE9 beta bugs with innerHTML. Test again with final IE9.
    svg = svg.replace(/(url\(#highcharts-[0-9]+)&quot;/g, '$1')
        .replace(/&quot;/g, "'");
    if (svg.match(/ xmlns="/g).length == 2) {
        svg = svg.replace(/xmlns="[^"]+"/, '');
    }

    return svg;
}
</script>
<style type="text/css">
@media (min-width: 800px) {
/*  .container {
    max-width: 800px;
  }
*/}
textarea {
  font-family: Consolas, 'Liberation Mono', Menlo, Courier, monospace;
}
.tablechart {
/*  width: 500px;
*/  margin: auto;
  padding-top: 20px;
  padding-bottom: 20px;
}
.chart {
  padding-top: 20px;
  padding-bottom: 20px;
}
body { padding-top: 70px; }
</style>
</head>
<body>
<div class="container">
<nav class="navbar navbar-default navbar-fixed-top" role="navigation">
  <div class="container">
    <!-- Brand and toggle get grouped for better mobile display -->
    <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="https://github.com/Loki-Astari/JSONBenchmark"><span class="glyphicon glyphicon-home"></span> JSONBenchmark</a>
    </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav">
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">Benchmark <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu" id="benchmark">
          </ul>
        </li>
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">Section <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu" id="section">
          </ul>
        </li>
      </ul>
      <p class="navbar-text navbar-right">Developed by <a href="https://github.com/miloyip" class="navbar-link">Milo Yip</a></p>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>
<div class="page-header">
<h1 id="title"><?=basename($argv[1], '.'.pathinfo($argv[1], PATHINFO_EXTENSION)).'.'.$argv[2]?></h1>
</div>
<div id="main"></div>
<h2>Source CSV</h2>
<textarea id="textInput" class="form-control" rows="5" readonly>
<?php include $argv[1] ?>
</textarea>
</div>
<div class="row" id="downloadDD" style="display: none">
<div class="btn-group pull-right" >
    <button class="btn dropdown-toggle" data-toggle="dropdown"><span class="glyphicon glyphicon-picture"></span></button>
    <ul class="dropdown-menu">
        <li><a tabindex="-1" href="#" dltype="image/jpeg">JPEG</a></li>
        <li><a tabindex="-1" href="#" dltype="image/png">PNG</a></li>
        <li><a tabindex="-1" href="#" dltype="application/pdf">PDF</a></li>
        <li><a tabindex="-1" href="#" dltype="image/svg+xml">SVG</a></li>
    </ul>
</div>
</div>
<form method="post" action="https://export.highcharts.com/" id="imageGetForm">
    <input type="hidden" name="filename" id="imageFilename" value="" />
    <input type="hidden" name="type" id="imageGetFormTYPE" value="" />
    <input type="hidden" name="width" value="1600" />
    <input type="hidden" name="svg" value="" id="imageGetFormSVG" />
</form>
</div>
</body>
</html>
