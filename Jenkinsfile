import org.apache.commons.lang.StringUtils

def unstableIfFound(String str) {
    def logs = currentBuild.rawBuild.getLog(10000).join('\n')
    int count = StringUtils.countMatches(logs, str);

    if (count > occurrence -1)
        currentBuild.result = 'UNSTABLE'
}

pipeline {
    agent { docker { image 'epitechcontent/epitest-docker' } }
    stages {
        stage('Build and Test') {
            steps {
                sh 'env'

                configFileProvider ([
                    configFile(
                        fileId: '405fdc1c-5b16-4ef3-a250-e6b6a35ba0a3',
                        targetLocation: '/home/jenkins/.epitechcli.json'
                    )
                ]) {
                    sh 'curl -o EpitechCLI-latest.jar https://blueslime.fr/epitechcli/EpitechCLI-latest.jar; java -jar EpitechCLI-latest.jar test'
                }
            }
        }
    }
    post {
        always {
            unstableIfFound 'Build failed'
            cobertura coberturaReportFile: '**/coverage.xml'
            junit '**/report.xml'
            archiveArtifacts artifacts: 'trace.txt'
        }
    }
}