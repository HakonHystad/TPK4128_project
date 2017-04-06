 <!DOCTYPE html>
 
<html>
<body>
  <h1>Available data</h1>

  <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.2.6/jquery.min.js" type="text/javascript" charset="utf-8"></script>

<style>
  table
  {
    font-family: arial, sans-serif;
    border-collapse: collapse;
    width: 100%;
  }

  td, th
  {
    border: 1px solid #dddddd;
    text-align: left;
    padding: 8px;
  }

  tr:nth-child(even)
  {
    background-color: #dddddd;
  }
</style>

  <div id="table"></div>

  <div id="error"></div>

<script type="text/javascript" charset="utf-8">
  function update()
  {
  $.ajax({
            type: "GET",
            url: "updateData.php",

            async: true, /* If set to non-async, browser shows page as "Loading.."*/
            cache: false,
            timeout:50000, /* Timeout in ms */

            success: function(data){ /* called when request completes */
                document.getElementById("table").innerHTML = data;
                setTimeout(
                    update(), /* Request next message */
                    2000 /* ..after 2 seconds */
                );
            },
            error: function(XMLHttpRequest, textStatus, errorThrown){
                document.getElementById("error").innerHTML = textStatus + " (" + errorThrown + ")";
                setTimeout(
                    update(), /* Try again after.. */
                    15000); /* milliseconds (15seconds) */
            }
        });
  };

  $(document).ready(function(){
        update(); /* Start the inital request */
    });
</script>





</body>
</html>
