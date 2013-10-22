
Dir.glob("./parser_tests/*/*.c") { |f|
    res = `./syntree_demo #{f} #{f}.res`
    if f =~ /w[0-9]+.c/
        # エラーありが正解
        unless res =~ /error/
            print "expected syntax error in #{f}\n"
        end
    elsif f =~ /f[0-9]+.c/
        # diff 一致が必要
        diff = `diff #{f}.ans #{f}.res`
        if diff != ""
            print "diff in #{f}\n"
            print diff
        end
    else
        # エラーなしが正解
        if res =~ /error/
            print "error in #{f}\n"
            print res + "\n"
        end
    end
    print "."
}


print "finished.\n"