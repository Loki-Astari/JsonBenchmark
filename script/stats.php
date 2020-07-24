<html>
<head>
<link rel="stylesheet" href="combine.css">
<script src="combine.js"></script>
<!--
<script src="https://code.jquery.com/jquery-1.8.2.js"></script>
<script src="https://www.google.com/jsapi?autoload={'modules':[{'name':'visualization','version':'1','packages':['controls', 'charteditor']}]}"></script>
<script src="https://jquery-csv.googlecode.com/git/src/jquery.csv.js"></script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap-theme.min.css">
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js"></script>
-->
<script>
$(function() {
  google.load("visualization", "1", {packages:["corechart"]});

  var csv = $('#textInput').val();
  var dt = google.visualization.arrayToDataTable($.csv.toArrays(csv, {onParseValue: $.csv.hooks.castToScalar}));

  function sortCaseInsensitive(dt, column) {
    for (var row = 0; row < dt.getNumberOfRows(); row++) {
      var s = dt.getValue(row, column);
      dt.setValue(row, column, s.toUpperCase());
      dt.setFormattedValue(row, column, s);
    }
    dt.sort(column);
  }

  function AddToOverall(dt) {
    if (overallDt == null) {
      overallDt = dt.clone();
    }
    else {
      var col1 = [];
      for (var i = 1; i < overallDt.getNumberOfColumns(); i++)
        col1.push(i);
      overallDt = google.visualization.data.join(overallDt, dt, 'left', [[0, 0]], col1, [1]);
    }

    var newCol = overallDt.getNumberOfColumns() - 1;
    overallDt.setColumnLabel(newCol, overallDt.getColumnLabel(newCol))
    //var formatter1 = new google.visualization.NumberFormat({ fractionDigits: 0 });
    //formatter1.format(overallDt, newCol);

  }

  addSection("0. Overall");

  var overallDiv = document.createElement("div");
  overallDiv.className = "tablechart";
  $("#main").append(overallDiv);

  // Per type sections
  var overallDt;
  for (var loop = 1; loop < dt.getNumberOfColumns(); ++loop) {
    var type = dt.getColumnLabel(loop);

    var view = new google.visualization.DataView(dt);
    view.setRows(view.getFilteredRows([{column: loop}]));

    console.log("Data: " + dt.getFormattedValue(4,3));

    var timedt = google.visualization.data.group(
        view,
        [0], 
        [{"column": loop, "aggregation": google.visualization.data.max, 'type': dt.getColumnType(loop) }]
    );

    AddToOverall(timedt);
  }

  var overallTable = new google.visualization.Table(overallDiv);
  sortCaseInsensitive(overallDt, 0);
  overallTable.draw(overallDt);

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
  var thisConfig = <?="\"".basename($argv[1], '.'.pathinfo($argv[1], PATHINFO_EXTENSION))."\""?>;
  var configurations = [ "Conformance.osx", "Conformance.linux", "Performance.osx", "Performance.linux", "Stats"];

  for (var i in configurations) {
    var c = configurations[i];
    $("#benchmark").append($("<li>", {class : (c == thisConfig ? "active" : "")}).append($("<a>", {href: c + ".html"}).append(c)));
  }
});

function addSection(name) {
  $("#main").append(
    $("<a>", {"name": name}),
    $("<h2>", {style: "padding-top: 70px; margin-top: -70px;"}).append(name)
  );
  $("#section").append($("<li>").append($("<a>", {href: "#" + name}).append(name)));
}

</script>
<style type="text/css">
@media (min-width: 800px) {
/*  .container {
    max-width: 800px;
  }*/
}
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
<h1 id="title"><?=basename($argv[1], '.'.pathinfo($argv[1], PATHINFO_EXTENSION))?></h1>
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
